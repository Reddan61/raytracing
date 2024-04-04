#include "AmbientLight.h"

AmbientLight::AmbientLight(float bright) : Light(glm::vec3(0, 0, 0), bright, 0)
{
}

glm::vec3 AmbientLight::getLightVector(glm::vec3& point)
{
	return this->getPosition();
}

bool AmbientLight::hasPosition()
{
	return false;
}

float AmbientLight::getMaxT()
{
	//return Infinity;
	return 0;
}
