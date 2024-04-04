#include "Light.h"

Light::Light(const glm::vec3 const &position, float bright, float shininess)
{
	this->bright = bright;
	this->shininess = shininess;
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

float Light::getShininess()
{
	return this->shininess;
}
