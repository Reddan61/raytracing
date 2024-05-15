#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const glm::vec3& const direction, float bright, float shininess) 
	: Light(glm::vec3(0.0f, 0.0f, 0.0f), bright, shininess)
{
	this->direction = direction;
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

DirectionalLight::DirectionalLightShader DirectionalLight::getBuffer()
{
	DirectionalLightShader result;

	result.bright = this->getBright();
	result.direction = glm::vec4(this->direction, 1.0f);
	result.shininess = this->getShininess();

	return result;
}
