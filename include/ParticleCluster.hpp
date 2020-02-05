#ifndef PARTICLE_CLUSTER_HPP_
#define PARTICLE_CLUSTER_HPP_

#include "Particle.hpp"
#include "ParticleConfiguration.hpp"

#include "SFML/Graphics/Drawable.hpp"

#include <vector>

namespace BinaryRing {

class ParticleCluster: public sf::Drawable {
    public:
        ParticleCluster(
            unsigned int clusterParticleIndex,
            unsigned int particlePerCluster,
            unsigned int globalParticleNumber,
            ParticleConfiguration configuration
        );

        void resize(sf::Vector2u dimensions) noexcept;
        void update(bool blackout) noexcept;
        void draw(
            sf::RenderTarget & target,
            sf::RenderStates states
        ) const final;
        std::size_t getParticleNumber() const noexcept;

    private:
        std::vector<Particle> particles_;
        ParticleConfiguration configuration_;
}; /* class ParticleCluster */

}; /* namespace BinaryRing */

#endif /* PARTICLE_CLUSTER_HPP_ */
