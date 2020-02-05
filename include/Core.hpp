#ifndef CORE_HPP_
#define CORE_HPP_

#include "ParticleCluster.hpp"
#include "ParticleConfiguration.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace BinaryRing {
    class Core {
        public:
            Core(
                sf::Vector2u dimensions,
                unsigned int particleNumber,
                unsigned int clusterNumber,
                unsigned int framerate,
                unsigned int maxAge
            );
            ~Core();

            int run();

        private:
            void updateClusters() noexcept;
            void handleEvents() noexcept;
            void switchBlackout() noexcept;
            void createClusters(
                unsigned int clusterNumber,
                unsigned int particleNumber,
                ParticleConfiguration configuration
            );

            void createClusterWorkers(std::size_t workerNumber) noexcept;
            void workerRoutine(std::size_t index) noexcept;

            // Window and program flow
            sf::RenderWindow window_;
            bool paused_ = false;

            // Particles
            std::vector<ParticleCluster> clusters_;
            bool blackout_;

            // Threads
            std::vector<bool> clusterState_;
            std::vector<std::thread> clusterWorkers_;
            std::condition_variable conditionVariable_;
            std::mutex mutex_;
            bool stop_ = false;
    };
};

#endif /* CORE_HPP_ */