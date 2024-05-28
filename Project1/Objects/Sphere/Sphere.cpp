#include "Sphere.h"

Sphere::Sphere(const glm::vec4& const position, const glm::vec4& const color, float radius, float specular, float reflective)
{
	this->position = position;
	this->color = color;
	this->radius = radius;
	this->specular = specular;
	this->reflective = reflective;
}

void Sphere::update(float delta)
{
	if (this->animation != nullptr) {
		bool updated = this->animation->update(delta);

		if (updated) {
			glm::vec4 new_pos = this->animation->getPosition();

			this->changePosition(new_pos);
		}

		this->is_updated = updated;
	}
}

void Sphere::changePosition(const glm::vec4 const& position)
{
	this->position = position;
}

glm::vec4 Sphere::getPosition()
{
	return this->position;
}

Sphere::SphereShader Sphere::getShader()
{
	SphereShader sphere;

	sphere.center = this->position;
	sphere.color = this->color;
	sphere.radius = this->radius;
	sphere.reflective = this->reflective;
	sphere.specular = this->specular;

	return sphere;
}

VkDeviceSize Sphere::getBufferSize()
{
	return sizeof(Sphere::SphereShader);
}

bool Sphere::isUpdated()
{
	return this->is_updated;
}
