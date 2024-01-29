#include "Math.h";

float Math::GetDotProduct(const Vector3d const &v1, const Vector3d const& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Math::LengthVector(const Vector3d const &vec)
{
	return std::sqrt(Math::GetDotProduct(vec, vec));
}

float Math::GetRadians(float degree)
{
	return degree * (3.14f / 180.f);
}

//Color Math::Multiply(float number, const Color const &color)
//{
//	float r = color.r * number >= 255 ? 255 : color.r * number;
//	float g = color.g * number >= 255 ? 255 : color.g * number;
//	float b = color.b * number >= 255 ? 255 : color.b * number;
//
//	return Color(r, g, b, 255);
//}

//Color Math::Add(const Color const &c1, const Color const &c2)
//{
//	int r = c1.r + c2.r >= 255 ? 255 : c1.r + c2.r;
//	int g = c1.g + c2.g >= 255 ? 255 : c1.g + c2.g;
//	int b = c1.b + c2.b >= 255 ? 255 : c1.b + c2.b;
//
//	return Color(r, g, b, 255);
//}

//Vector3d Math::Add(const Vector3d const& c1, const Color const& c2)
//{
//	return Vector3d(c1.x + c2.r, c1.y + c2.g, c1.z + c2.b);
//}

Vector3d Math::GetCrossProduct(const Vector3d const& v1, const Vector3d const& v2)
{
	Vector3d result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

Matrix4d Math::GetLookAt(const Vector3d const& from, const Vector3d const& to, const Vector3d const& up)
{
	Vector3d forward = from - to;
	forward = forward.normalize();

	Vector3d right = Math::GetCrossProduct(up, forward);
	right = right.normalize();

	Vector3d newUp = Math::GetCrossProduct(forward, right);

	Matrix4d rotation;

	rotation.setItem(0, 0, right.x);
	rotation.setItem(0, 1, right.y);
	rotation.setItem(0, 2, right.z);

	rotation.setItem(1, 0, newUp.x);
	rotation.setItem(1, 1, newUp.y);
	rotation.setItem(1, 2, newUp.z);

	rotation.setItem(2, 0, forward.x);
	rotation.setItem(2, 1, forward.y);
	rotation.setItem(2, 2, forward.z);

	rotation.setItem(3, 3, 1);
	
	Matrix4d translation;
	
	translation.setItem(0, 0, 1);
	translation.setItem(1, 1, 1);
	translation.setItem(2, 2, 1);
	translation.setItem(3, 3, 1);

	translation.setItem(0, 3, -from.x);
	translation.setItem(1, 3, -from.y);
	translation.setItem(2, 3, -from.z);

	return rotation * translation;
}

double Math::GetRandom()
{
	return rand() / (RAND_MAX + 1.0);
}

double Math::GetRandomRange(double min, double max)
{
	return min + (max - min) * Math::GetRandom();
}

//Vector3d Math::normalize(const Vector3d const& vec)
//{
//	float inv_length = 1.0f / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
//
//	return vec * inv_length;
//}
