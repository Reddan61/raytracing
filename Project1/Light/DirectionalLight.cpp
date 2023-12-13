#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const sf::Vector3f& const position, float bright) : Light(position, bright)
{
}

sf::Vector3f DirectionalLight::getLightVector(sf::Vector3f& point)
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
