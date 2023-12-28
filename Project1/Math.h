#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "./Matrix/Matrix3d.h"
#include "./Matrix/Matrix4d.h"

#define Infinity std::numeric_limits<int>::max()

namespace Math {
	float GetDotProduct(const sf::Vector3f& const v1, const sf::Vector3f& v2);
	float LengthVector(sf::Vector3f &vec);
	float GetRadians(float degree);

	sf::Color Multiply(float number, const sf::Color & const color);
	sf::Color Add(const sf::Color  & const c1, const sf::Color & const c2);
	sf::Vector3f GetCrossProduct(const sf::Vector3f const& v1, const sf::Vector3f const& v2);
	sf::Vector3f normalize(const sf::Vector3f const &vec);
	Matrix4d GetLookAt(const sf::Vector3f const& from, const sf::Vector3f const& to, const sf::Vector3f const& up);
	//Matrix3d getLookAt();
}