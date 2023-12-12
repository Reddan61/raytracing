#include "Light.h"

Light::Light(const sf::Vector3f const &position, float bright)
{
	this->bright = bright;
	this->position = position;
}

float Light::getBright()
{
	return this->bright;
}

sf::Vector3f Light::getPosition()
{
	return this->position;
}
