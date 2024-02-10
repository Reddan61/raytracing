#include "AmbientLight.h"

AmbientLight::AmbientLight(float bright) : Light(Vector3d(Infinity, Infinity, Infinity), bright)
{
}

Vector3d AmbientLight::getLightVector(Vector3d& point)
{
	return this->getPosition();
}

bool AmbientLight::hasPosition()
{
	return false;
}

float AmbientLight::getMaxT()
{
	return Infinity;
}

C99AmbientLight* AmbientLight::getC99()
{
	C99AmbientLight *result = new C99AmbientLight;

	result->bright = this->getBright();

	return result;
}
