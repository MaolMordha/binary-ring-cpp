#ifndef PARTICLE_CONFIGURATION_HPP_
#define PARTICLE_CONFIGURATION_HPP_

#include <SFML/System/Vector2.hpp>

namespace BinaryRing {

struct ParticleConfiguration {
    sf::Vector2f origin;
    unsigned int maxAge;
    bool blackout;
}; /* namespace BinaryRing */

}; /* struct ParticleConfiguration */

#endif /* PARTICLE_CONFIGURATION_HPP_ */