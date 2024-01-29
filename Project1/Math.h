#pragma once
#include <iostream>
#include <cstdlib>
#include "./Utils/Matrix/Matrix3d.h"
#include "./Utils/Matrix/Matrix4d.h"
#include "./Utils/Color/Color.h"
#include "./Utils/Vector/Vector3d.h"

#define Infinity std::numeric_limits<int>::max()
#define Epsilon std::numeric_limits<float>::epsilon()

namespace Math {
	float GetDotProduct(const Vector3d const& v1, const Vector3d const& v2);
	float LengthVector(const Vector3d const& vec);
	float GetRadians(float degree);

	//sf::Color Multiply(float number, const Color const& color);
	//sf::Color Add(const Color  const& c1, const Color const& c2);
	//Vector3d Add(const Vector3d const& c1, const Color const& c2);
	Vector3d GetCrossProduct(const Vector3d const& v1, const Vector3d const& v2);
	//Vector3d normalize(const Vector3d const& vec);
	Matrix4d GetLookAt(const Vector3d const& from, const Vector3d const& to, const Vector3d const& up);

	double GetRandom();
	double GetRandomRange(double min, double max);
}