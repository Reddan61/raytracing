#include "Vector3d.h"

Vector3d::Vector3d()
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Vector3d::Vector3d(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3d::Vector3d(const Vector3d& other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
}

Vector3d::~Vector3d()
{
}

C99Vector Vector3d::getC99()
{
	C99Vector vec;

	vec.x = this->x;
	vec.y = this->y;
	vec.z = this->z;

	return vec;
}

Vector3d Vector3d::normalize() const
{
	float inv_length = 1.0f / this->length();

	return *this * inv_length;
}

float Vector3d::length() const
{
	return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

Vector3d& Vector3d::operator=(const Vector3d& other)
{
	if (&other != this) {
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
	}

	return *this;
}

Vector3d& Vector3d::operator+=(const Vector3d& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;

	return *this;
}

Vector3d& Vector3d::operator-=(const Vector3d& other)
{
	this->x -= other.x;
	this->y -= other.y;
	this->z	-= other.z;

	return *this;
}

float& Vector3d::operator[](int index)
{
	switch (index) {
		case 0: return this->x;
		case 1: return this->y;
		case 2: return this->z;
		default: return this->x;
	};
}

Vector3d operator+(const Vector3d& left, const Color& right)
{
	return Vector3d(
		left.x + right.r,
		left.y + right.g,
		left.z + right.b
	);
}

Vector3d operator+(const Vector3d& left, const Vector3d& right)
{
	return Vector3d(
		left.x + right.x,
		left.y + right.y,
		left.z + right.z
	);
}

Vector3d operator-(const Vector3d& left, const Vector3d& right)
{
	return Vector3d(
		left.x - right.x,
		left.y - right.y,
		left.z - right.z
	);
}

Vector3d operator* (float left, const Vector3d& right)
{
	return Vector3d(
		right.x * left,
		right.y * left,
		right.z * left
	);
}

Vector3d operator*(const Vector3d& left, float right)
{
	return Vector3d(
		left.x * right,
		left.y * right,
		left.z * right
	);
}

Vector3d operator*(const Vector3d& left, const Vector3d& right)
{
	return Vector3d(
		left.x * right.x,
		left.y * right.y,
		left.z * right.z
	);
}

Vector3d operator/(const Vector3d& left, const float right)
{
	return Vector3d(
		left.x / right,
		left.y / right,
		left.z / right
	);
}
