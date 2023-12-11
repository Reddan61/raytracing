#include "Light.h"

Light::Light(float bright)
{
	this->bright = bright;
}

float Light::getBright()
{
	return this->bright;
}
