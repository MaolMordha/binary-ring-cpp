#include "ParticleCluster.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <cmath>

using namespace BinaryRing;

ParticleCluster::ParticleCluster(
    unsigned int clusterParticleIndex,
    unsigned int particlePerCluster,
    unsigned int globalParticleNumber,
    ParticleConfiguration configuration
):
    configuration_(configuration)
{
    particles_.reserve(particlePerCluster);
    particlePerCluster += clusterParticleIndex;

    for (unsigned int i = clusterParticleIndex; i < particlePerCluster; ++i) {
        double arg = 2.0 * std::acos(-1) * (i / globalParticleNumber);

        particles_.emplace_back(
            configuration_,
            sf::Vector2f(30.0 * std::sin(arg), 30.0 * std::cos(arg)),
            (std::acos(-1) * i) / globalParticleNumber
        );
    }
}

void ParticleCluster::resize(sf::Vector2u dimensions) noexcept
{
    configuration_.origin = {
        static_cast<float>(dimensions.x) / 2,
        static_cast<float>(dimensions.y) / 2
    };
    configuration_.blackout = false;
    for (auto & particle: particles_) {
        particle.reset();
    }
}

void ParticleCluster::update(bool blackout) noexcept
{
    configuration_.blackout = blackout;
    for (auto & particle: particles_) {
        particle.update();
    }
}

void ParticleCluster::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    for (const auto & particle: particles_) {
        target.draw(particle);
    }
}

std::size_t ParticleCluster::getParticleNumber() const noexcept
{
    return particles_.size();
}
