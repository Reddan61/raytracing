#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec4& const direction, float bright, float shininess) 
	: Light(bright, shininess)
{
	this->direction = direction;
}

glm::vec4 DirectionalLight::getDirection()
{
	return this->direction;
}

glm::vec4 DirectionalLight::getLightVector(glm::vec4& point)
{
	return this->getDirection();
}

float DirectionalLight::getMaxT()
{
	return 0;
}

DirectionalLight::DirectionalLightShader DirectionalLight::getBuffer()
{
	DirectionalLightShader result;

	result.bright = this->getBright();
	result.direction = this->direction;
	result.shininess = this->getShininess();

	return result;
}
