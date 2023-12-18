#include "Math.h";

float Math::GetDotProduct(const sf::Vector3f& const v1, const sf::Vector3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float Math::LengthVector(sf::Vector3f& vec)
{
	return std::sqrt(Math::GetDotProduct(vec, vec));
}

sf::Color Math::Multiply(float number, const sf::Color& const color)
{
	int r = color.r * number >= 255 ? 255 : color.r * number;
	int g = color.g * number >= 255 ? 255 : color.g * number;
	int b = color.b * number >= 255 ? 255 : color.b * number;

	return sf::Color(r, g, b, 255);
}

sf::Color Math::Add(const sf::Color& const c1, const sf::Color& const c2)
{
	int r = c1.r + c2.r >= 255 ? 255 : c1.r + c2.r;
	int g = c1.g + c2.g >= 255 ? 255 : c1.g + c2.g;
	int b = c1.b + c2.b >= 255 ? 255 : c1.b + c2.b;

	return sf::Color(r, g, b, 255);
}
