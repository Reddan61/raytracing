#include "Math.h";

const sf::Vector3f& Math::subTract(const sf::Vector3f& v1, const sf::Vector3f& v2) {
	return sf::Vector3f(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

float Math::GetDotProduct(const sf::Vector3f& v1, const sf::Vector3f& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
