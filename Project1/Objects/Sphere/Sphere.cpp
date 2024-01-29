#include "Sphere.h"

Sphere::Sphere(const Vector3d& const position, const Color& const color, int radius, float specular, float reflective)
{
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->specular = specular;
	this->reflective = reflective;
}

Vector3d Sphere::getNormal(const Vector3d const& point, const Vector3d const& direction)
{
	Vector3d normal = point - this->position;
	normal = normal.normalize();

	return normal;
}

void Sphere::update(sf::RenderWindow& window, sf::Time time)
{
}

void Sphere::changePosition(const Vector3d const& position)
{
	this->position = position;
}

Vector3d Sphere::getPosition()
{
	return this->position;
}

C99Sphere Sphere::getC99()
{
	C99Sphere result;

	result.color = this->color.getC99();
	result.position = this->getPosition().getC99();
	result.radius = this->radius;
	result.reflective = this->getReflective();
	result.specular = this->getSpecular();

	return result;
}

void Sphere::render(sf::RenderWindow& window)
{
}

Object::InsertRayValue Sphere::insertRay(Vector3d& cameraPosition, Vector3d& direction)
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
