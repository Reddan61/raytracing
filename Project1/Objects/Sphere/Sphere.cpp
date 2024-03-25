#include "Sphere.h"

Sphere::Sphere(const glm::vec3& const position, const glm::vec3& const color, float radius, float specular, float reflective)
{
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->specular = specular;
	this->reflective = reflective;
}

glm::vec3 Sphere::getNormal(const glm::vec3 const& point, const glm::vec3 const& direction)
{
	glm::vec3 normal = point - this->position;
	normal = glm::normalize(normal);

	return normal;
}

void Sphere::update(GLFWwindow* window, float delta)
{
}

void Sphere::changePosition(const glm::vec3 const& position)
{
	this->position = position;
}

glm::vec3 Sphere::getPosition()
{
	return this->position;
}

Sphere::SphereShader Sphere::getSphereShader()
{
	SphereShader sphere;

	sphere.center = glm::vec4(this->position, 1.0f);
	sphere.color = glm::vec4(this->color, 1.0f);
	sphere.radius = this->radius;
	sphere.reflective = this->reflective;
	sphere.specular = this->specular;

	return sphere;
}