#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#define Infinity std::numeric_limits<int>::max()

namespace Math {
	float GetDotProduct(const sf::Vector3f& const v1, const sf::Vector3f& v2);
	float LengthVector(sf::Vector3f &vec);
	
	sf::Color Multiply(float number, const sf::Color & const color);
	sf::Color Add(const sf::Color  & const c1, const sf::Color & const c2);
}