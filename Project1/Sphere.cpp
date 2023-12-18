#include "Sphere.h"

Sphere::Sphere(const sf::Vector3f& const position, const sf::Color& const color, int radius, float specular, float reflective)
{
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->specular = specular;
	this->reflective = reflective;
}

void Sphere::update(sf::RenderWindow& window, sf::Time time)
{
}

void Sphere::render(sf::RenderWindow& window)
{
}

sf::Vector2f Sphere::insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction)
{
	auto oc = cameraPosition - this->getPosition();

	float a = Math::GetDotProduct(direction, direction);
	float b = 2.f * Math::GetDotProduct(oc, direction);
	float c = Math::GetDotProduct(oc, oc) - this->radius * this->radius;

	float discriminant = b * b - 4.f * a * c;

	if (discriminant < 0) {
		return sf::Vector2f(Infinity, Infinity);
	}

	float x1 = (-b + std::sqrt(discriminant)) / (2 * a);
	float x2 = (-b - std::sqrt(discriminant)) / (2 * a);

	return sf::Vector2f(x1, x2);
}
