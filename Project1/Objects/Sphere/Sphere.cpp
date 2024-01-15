#include "Sphere.h"

Sphere::Sphere(const sf::Vector3f& const position, const sf::Color& const color, int radius, float specular, float reflective)
{
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->specular = specular;
	this->reflective = reflective;
}

sf::Vector3f Sphere::getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction)
{
	sf::Vector3f normal = point - this->position;
	normal = Math::normalize(normal);

	return normal;
}

void Sphere::update(sf::RenderWindow& window, sf::Time time)
{
}

void Sphere::changePosition(const sf::Vector3f const& position)
{
	this->position = position;
}

void Sphere::render(sf::RenderWindow& window)
{
}

Object::InsertRayValue Sphere::insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction)
{
	auto oc = cameraPosition - this->position;

	float a = Math::GetDotProduct(direction, direction);
	float b = 2.f * Math::GetDotProduct(oc, direction);
	float c = Math::GetDotProduct(oc, oc) - this->radius * this->radius;

	float discriminant = b * b - 4.f * a * c;

	if (discriminant < 0) {
		return Object::InsertRayValue(Infinity, Infinity, nullptr);
	}

	float x1 = (-b + std::sqrt(discriminant)) / (2 * a);
	float x2 = (-b - std::sqrt(discriminant)) / (2 * a);

	return Object::InsertRayValue(x1, x2, this);
}
