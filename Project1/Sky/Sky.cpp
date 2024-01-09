#include "Sky.h"

sf::Color Sky::getColor(const sf::Vector3f &direction)
{
    sf::Color base(133, 249, 255, 255);
    sf::Vector3f normalized = Math::normalize(direction);

    float pos = 0.5 * (normalized.y + 1.0);

    return Math::Multiply(pos, base) + Math::Multiply((1 - pos), sf::Color(255, 255, 255));
}
