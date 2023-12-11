#pragma once
#include <SFML/Graphics.hpp>

#define Infinity std::numeric_limits<int>::max()

namespace Math {
	float GetDotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2);
	float LengthVector(sf::Vector3f &vec);
	
	const sf::Color& Multiply(float number, const sf::Color & const color);
}