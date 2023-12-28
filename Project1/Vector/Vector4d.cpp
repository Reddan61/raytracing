#include "Vector4d.h"

Vector4d::Vector4d()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->w = 0;
}

Vector4d::Vector4d(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4d::Vector4d(const sf::Vector3f const &vec, float w)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = vec.z;
	this->w = w;
}

Vector4d::~Vector4d()
{
}

float& Vector4d::operator[](int index)
{
	switch (index) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		default: return this->w;
	};
}
