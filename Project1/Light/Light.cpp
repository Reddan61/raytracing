#include "Light.h"

Light::Light(const glm::vec3 const &position, float bright)
{
	this->bright = bright;
	this->position = position;
}

float Light::getBright()
{
	return this->bright;
}

glm::vec3 Light::getPosition()
{
	return this->position;
}
