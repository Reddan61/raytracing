#include "Math.h";

float Math::GetDotProduct(const sf::Vector3f& const v1, const sf::Vector3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Math::LengthVector(sf::Vector3f& vec)
{
	return std::sqrt(Math::GetDotProduct(vec, vec));
}

float Math::GetRadians(float degree)
{
	return degree * (3.14 / 180);
}

sf::Color Math::Multiply(float number, const sf::Color const &color)
{
	float r = color.r * number >= 255 ? 255 : color.r * number;
	float g = color.g * number >= 255 ? 255 : color.g * number;
	float b = color.b * number >= 255 ? 255 : color.b * number;

	return sf::Color(r, g, b, 255);
}

sf::Color Math::Add(const sf::Color const &c1, const sf::Color const &c2)
{
	int r = c1.r + c2.r >= 255 ? 255 : c1.r + c2.r;
	int g = c1.g + c2.g >= 255 ? 255 : c1.g + c2.g;
	int b = c1.b + c2.b >= 255 ? 255 : c1.b + c2.b;

	return sf::Color(r, g, b, 255);
}

sf::Vector3f Math::Add(const sf::Vector3f const& c1, const sf::Color const& c2)
{
	return sf::Vector3f(c1.x + c2.r, c1.y + c2.g, c1.z + c2.b);
}

sf::Vector3f Math::GetCrossProduct(const sf::Vector3f const& v1, const sf::Vector3f const& v2)
{
	sf::Vector3f result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.x * v2.z - v1.z * v2.x;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
}

Matrix4d Math::GetLookAt(const sf::Vector3f const& from, const sf::Vector3f const& to, const sf::Vector3f const& up)
{
	sf::Vector3f forward = from - to;
	forward = Math::normalize(forward);

	sf::Vector3f right = Math::GetCrossProduct(up, forward);
	right = Math::normalize(right);

	sf::Vector3f newUp = Math::GetCrossProduct(forward, right);

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

sf::Vector3f Math::normalize(const sf::Vector3f const& vec)
{
	float inv_length = 1.0f / sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

	return vec * inv_length;
}
