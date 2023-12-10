#pragma once
#include <SFML/Graphics.hpp>

#define Infinity std::numeric_limits<int>::max()

namespace Math {
	const sf::Vector3f& subTract(const sf::Vector3f& v1, const sf::Vector3f& v2);
	float GetDotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2);
}