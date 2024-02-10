#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const Vector3d& const position, float bright) : Light(position, bright)
{
}

Vector3d DirectionalLight::getLightVector(Vector3d& point)
{
	return this->getPosition();
}

bool DirectionalLight::hasPosition()
{
	return true;
}

float DirectionalLight::getMaxT()
{
	return Infinity;
}

C99DirectionalLight DirectionalLight::getC99()
{
	C99DirectionalLight result;

	result.bright = this->getBright();
	result.maxT = this->getMaxT();
	result.position = this->getPosition().getC99();

	return result;
}
