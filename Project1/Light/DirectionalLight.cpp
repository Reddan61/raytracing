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
