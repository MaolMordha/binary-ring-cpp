#ifndef PARTICLE_HPP_
#define PARTICLE_HPP_

#include "ParticleConfiguration.hpp"

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

namespace BinaryRing {

class Particle: public sf::Drawable {
    public:
        Particle(
            const ParticleConfiguration & configuration,
            sf::Vector2f offset,
            double arg
        );

        void reset() noexcept;
        void update() noexcept;
        virtual void draw(
            sf::RenderTarget & target,
            sf::RenderStates states
        ) const final;
        
    private:
        const ParticleConfiguration & configuration_;
        sf::Vertex points_[2];
        sf::Vertex rpoints_[2];
        sf::Vector2f velocity_;
        unsigned int age_;
}; /* class Particle */

}; /* namespace BinaryRing */

#endif /* PARTICLE_HPP_ */
