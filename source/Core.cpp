#include "Core.hpp"

#include "Exceptions/Exception.hpp"
#include "ParticleConfiguration.hpp"
#include "RandomUtils.hpp"

#include <cmath>
#include <SFML/Window.hpp>

#include <iostream>

using namespace BinaryRing;

Core::Core(
    sf::Vector2u dimensions,
    unsigned int particleNumber,
    unsigned int clusterNumber,
    unsigned int framerate,
    unsigned int maxAge
):
    window_({ dimensions.x, dimensions.y, 32 }, "Binary ring", sf::Style::Fullscreen, sf::ContextSettings(24, 8, 16)),
    clusterWorkers_(4),
    blackout_(false)
{
    std::srand(std::time(NULL));
    unsigned int screenWidth = sf::VideoMode::getDesktopMode().width;
    unsigned int screenHeight = sf::VideoMode::getDesktopMode().height;

    if (dimensions.x > screenWidth) {
        dimensions.x = screenWidth;
    }
    if (dimensions.y > screenHeight) {
        dimensions.y = screenHeight;
    }

    sf::Vector2i windowPosition = {
        static_cast<int>((screenWidth - dimensions.x) / 2u),
        static_cast<int>((screenHeight - dimensions.y) / 2u)
    };

    window_.setPosition(windowPosition);
    window_.setFramerateLimit(framerate);

    ParticleConfiguration configuration;

    configuration.maxAge = maxAge,
    configuration.origin = {
        static_cast<float>(dimensions.x) / 2,
        static_cast<float>(dimensions.y) / 2
    };
    configuration.blackout = false;
    createClusters(clusterNumber, particleNumber, configuration);
    createClusterWorkers(10);
}

Core::~Core()
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        stop_ = true;
    }
    conditionVariable_.notify_all();

    for (auto & worker: clusterWorkers_) {
        worker.join();
    }
}

int Core::run()
{
    while (window_.isOpen()) {
        handleEvents();
        if (!paused_) {
            switchBlackout();
            updateClusters();
            for (auto & cluster: clusters_) {
                window_.draw(cluster);
            }
        }
        window_.display();
    }
    return 0;
}

void Core::updateClusters() noexcept
{
    {
        std::lock_guard<std::mutex> lock(mutex_);

        std::fill(clusterState_.begin(), clusterState_.end(), true);
    }
    conditionVariable_.notify_all();

    {
        std::unique_lock<std::mutex> lock(mutex_);
        
        conditionVariable_.wait(lock, [this] {
            return std::find(
                this->clusterState_.begin(),
                this->clusterState_.end(),
                true
            ) == this->clusterState_.end();
        });
    }
}

void Core::handleEvents() noexcept
{
    sf::Event event;
    
    while (window_.pollEvent(event)) {
        switch (event.type) {
            case sf::Event::MouseButtonPressed:
                blackout_ = !blackout_;
                break;
            case sf::Event::Resized:
                for (auto & cluster: clusters_) {
                    cluster.resize(
                        { event.size.width, event.size.height },
                        blackout_
                    );
                }
                break;
            case sf::Event::Closed:
                window_.close();
                break;
            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::Key::Escape) {
                    window_.close();
                } else if (event.key.code == sf::Keyboard::Key::Space) {
                    paused_ = !paused_;
                }
                break;
            default:
                break;
        }
    }
}

void Core::switchBlackout() noexcept
{
    if (random(0, 10000) > 9900) {
        blackout_ = !blackout_;
    }
}

void Core::createClusters(
    unsigned int clusterNumber,
    unsigned int particleNumber,
    ParticleConfiguration configuration
) {
    if (clusterNumber > particleNumber) {
        throw Exception(
            "Cluster number must be equal or greater than particle number"
        );
    }
    unsigned int particlePerCluster = static_cast<unsigned int>(std::floor(
        static_cast<double>(particleNumber) / static_cast<double>(clusterNumber)
    ));

    clusters_.reserve(clusterNumber);
    for (unsigned int i = 0; i < clusterNumber - 1; ++i) {
        clusters_.emplace_back(
            i * particlePerCluster,
            particlePerCluster,
            particleNumber,
            configuration
        );
    }
    clusters_.emplace_back(
            (clusterNumber - 1) * particlePerCluster,
            particlePerCluster + particleNumber % clusterNumber,
            particleNumber,
            configuration
    );
}

void Core::createClusterWorkers(std::size_t workerNumber) noexcept
{
    clusterState_.resize(workerNumber, false);
    for (std::size_t i = 0; i < workerNumber; ++i) {
        clusterWorkers_.emplace_back(&Core::workerRoutine, this, i);
    }
}

void Core::workerRoutine(std::size_t index) noexcept
{
    while (true) {
        std::size_t index = 0;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            conditionVariable_.wait(lock, [this] {
                return this->stop_ || std::find(
                    this->clusterState_.begin(),
                    this->clusterState_.end(),
                    true
                ) != this->clusterState_.end();
            });
            if (this->stop_) {
                return;
            }
            auto it = std::find(
                clusterState_.begin(),
                clusterState_.end(),
                true
            );

            index = it - clusterState_.begin();
            *it = false;
            std::cout << "Thread #" << index << " processing "
                << clusters_[index].getParticleNumber()
                << " particles"
                << std::endl;
        }
        clusters_[index].update(blackout_);
        conditionVariable_.notify_one();
    }
}
