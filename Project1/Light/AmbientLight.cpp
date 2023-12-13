#include "AmbientLight.h"

AmbientLight::AmbientLight(float bright) : Light(sf::Vector3f(Infinity, Infinity, Infinity), bright)
{
}

sf::Vector3f AmbientLight::getLightVector(sf::Vector3f& point)
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
