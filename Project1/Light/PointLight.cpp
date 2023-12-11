#include "PointLight.h"

PointLight::PointLight(const sf::Vector3f& const position, float bright): Light(bright)
{
	this->position = position;
}

sf::Vector3f PointLight::getLightVector(sf::Vector3f& point)
{
	return this->position - point;
}
