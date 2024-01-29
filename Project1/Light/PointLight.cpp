#include "PointLight.h"

PointLight::PointLight(const Vector3d& const position, float bright): Light(position, bright)
{
}

Vector3d PointLight::getLightVector(Vector3d& point)
{
	return this->getPosition() - point;
}

bool PointLight::hasPosition()
{
	return true;
}

float PointLight::getMaxT()
{
	return 1.0f;
}
