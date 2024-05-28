#include "Light.h"

Light::Light(float bright, float shininess)
{
	this->bright = bright;
	this->shininess = shininess;
}

float Light::getBright()
{
	return this->bright;
}

float Light::getShininess()
{
	return this->shininess;
}
