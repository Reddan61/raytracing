#include "PointLight.h"

PointLight::PointLight(const sf::Vector3f& const position, float bright): Light(position, bright)
{
}

sf::Vector3f PointLight::getLightVector(sf::Vector3f& point)
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
