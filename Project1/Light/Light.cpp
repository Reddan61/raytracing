#include "Light.h"

Light::Light(const Vector3d const &position, float bright)
{
	this->bright = bright;
	this->position = position;
}

float Light::getBright()
{
	return this->bright;
}

Vector3d Light::getPosition()
{
	return this->position;
}
