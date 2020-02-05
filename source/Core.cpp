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
}

int Core::run()
{
    while (window_.isOpen()) {
        handleEvents();
        if (!paused_) {
            switchBlackout();
            for (auto & cluster: clusters_) {
                cluster.update(blackout_);
                window_.draw(cluster);
            }
        }
        window_.display();
    }
    return 0;
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
                    cluster.resize({ event.size.width, event.size.height });
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
