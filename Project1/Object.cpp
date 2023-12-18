#include "Object.h"

sf::Vector3f Object::getPosition()
{
	return this->position;
}

sf::Color Object::getColor()
{
	return this->color;
}

float Object::getSpecular()
{
	return this->specular;
}

float Object::getReflective()
{
	return this->reflective;
}


