#include "AmbientLight.h"

AmbientLight::AmbientLight(float bright)
{
	this->bright = bright;
}

float AmbientLight::getBright()
{
	return this->bright;
}
