#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include "./Matrix/Matrix3d.h"
#include "./Matrix/Matrix4d.h"

#define Infinity std::numeric_limits<int>::max()
#define Epsilon std::numeric_limits<float>::epsilon()

namespace Math {
	float GetDotProduct(const sf::Vector3f const& v1, const sf::Vector3f const& v2);
	float LengthVector(const sf::Vector3f const& vec);
	float GetRadians(float degree);

	sf::Color Multiply(float number, const sf::Color const& color);
	sf::Color Add(const sf::Color  const& c1, const sf::Color const& c2);
	sf::Vector3f Add(const sf::Vector3f  const& c1, const sf::Color const& c2);
	sf::Vector3f GetCrossProduct(const sf::Vector3f const& v1, const sf::Vector3f const& v2);
	sf::Vector3f normalize(const sf::Vector3f const& vec);
	Matrix4d GetLookAt(const sf::Vector3f const& from, const sf::Vector3f const& to, const sf::Vector3f const& up);

	double GetRandom();
	double GetRandomRange(double min, double max);
}