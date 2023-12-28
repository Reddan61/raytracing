#pragma once
#include <SFML/Graphics.hpp>

class Vector4d
{
public:
	Vector4d();
	Vector4d(float x, float y, float z, float w);
	Vector4d(const sf::Vector3f const &vec, float w = 1.f);
	~Vector4d();

	float& operator [](int index);
private:
	float x, y, z, w = 0;
};

