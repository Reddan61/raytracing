#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& const position, float bright, float shininess) : Light(position, bright, shininess)
{
}

glm::vec3 DirectionalLight::getLightVector(glm::vec3& point)
{
	return this->getPosition();
}

bool DirectionalLight::hasPosition()
{
	return true;
}

float DirectionalLight::getMaxT()
{
	//return Infinity;
	return 0;
}
