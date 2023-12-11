#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const sf::Vector3f& const position, float bright) : Light(bright)
{
	this->position = position;
}

sf::Vector3f DirectionalLight::getLightVector(sf::Vector3f& point)
{
	return this->position;
}
