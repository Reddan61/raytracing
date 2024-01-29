#pragma once
#include <cstdlib>
#include "../Color/Color.h"
#include "../../C99/structs.h"

class Color;

class Vector3d
{
public:
	Vector3d();
	Vector3d(float x, float y, float z);
	Vector3d(const Vector3d& other);
	~Vector3d();

	C99Vector getC99();

	Vector3d normalize() const;
	float length() const;


	Vector3d& operator =(const Vector3d& other);

	Vector3d& operator +=(const Vector3d& other);
	Vector3d& operator -=(const Vector3d& other);

	float& operator [](int index);
	float x, y, z = 0;
};

Vector3d operator +(const Vector3d& left, const Color& right);
Vector3d operator +(const Vector3d& left, const Vector3d& right);

Vector3d operator -(const Vector3d& left, const Vector3d& right);

Vector3d operator *(const float left, const Vector3d& right);
Vector3d operator *(const Vector3d& left, const float right);
Vector3d operator *(const Vector3d& left, const Vector3d& right);

Vector3d operator /(const Vector3d& left, const float right);


