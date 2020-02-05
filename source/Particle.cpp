#include "Particle.hpp"

#include "RandomUtils.hpp"

#include <cmath>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace BinaryRing;

Particle::Particle(
    const ParticleConfiguration & configuration,
    sf::Vector2f offset,
    double arg
):
    configuration_(configuration),
    velocity_({
        2.0f * static_cast<float>(std::cos(arg)),
        2.0f * static_cast<float>(std::sin(arg))
    }),
    age_(random(0, 200))
{
    points_[1].position = {
        configuration.origin.x - offset.x,
        configuration.origin.y - offset.y
    };
    rpoints_[1].position = {
        configuration.origin.x + offset.x,
        configuration.origin.y - offset.y
    };
    points_[0].color = sf::Color::White;
    points_[1].color = sf::Color::White;
    rpoints_[0].color = sf::Color::White;
    rpoints_[1].color = sf::Color::White;
}

void Particle::update() noexcept
{
    if (age_++ > configuration_.maxAge) {
        reset();
    }
    
    points_[0].position = points_[1].position;
    rpoints_[0].position = rpoints_[1].position;
    points_[1].position += velocity_;
    rpoints_[1].position += { -velocity_.x, velocity_.y };

    velocity_.x += (frandom(0.0, 100.0) - frandom(0.0, 100.0)) * 0.005;
    velocity_.y += (frandom(0.0, 100.0) - frandom(0.0, 100.0)) * 0.005;
}

void Particle::reset() noexcept
{
    float t = frandom(0.0, std::acos(-1) * 2.0);

    velocity_ = { 0.0, 0.0 };
    points_[1].position =
        configuration_.origin + sf::Vector2f(30 * std::sin(t), 30 * std::cos(t));
    rpoints_[1].position =
        configuration_.origin + sf::Vector2f(-30 * std::sin(t), 30 * std::cos(t));
    age_ = random(0, 30);

    if (configuration_.blackout) {
        points_[0].color = sf::Color::Black;
        points_[1].color = sf::Color::Black;
        rpoints_[0].color = sf::Color::Black;
        rpoints_[1].color = sf::Color::Black;
    } else {
        points_[0].color = sf::Color::White;
        points_[1].color = sf::Color::White;
        rpoints_[0].color = sf::Color::White;
        rpoints_[1].color = sf::Color::White;
    }
}

void Particle::draw(sf::RenderTarget & target, sf::RenderStates) const
{
    target.draw(points_, 2, sf::Lines);
    target.draw(rpoints_, 2, sf::Lines);
}
