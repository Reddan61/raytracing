#include "Math.h";

float Math::GetDotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Math::LengthVector(sf::Vector3f& vec)
{
	return std::sqrt(Math::GetDotProduct(vec, vec));
}

const sf::Color& Math::Multiply(float number, const sf::Color& const color)
{
	auto r = color.r * number > 255 ? 255 : color.r * number;
	auto g = color.g * number > 255 ? 255 : color.g * number;
	auto b = color.b * number > 255 ? 255 : color.b * number;

	return sf::Color(r, g, b, color.a);
}
