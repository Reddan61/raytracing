#include "PointLight.h"

PointLight::PointLight(const glm::vec4& const position, float bright, float shininess): Light(bright, shininess)
{
	this->position = position;
}

glm::vec4 PointLight::getLightVector(glm::vec4& point)
{
	return this->getPosition() - point;
}

float PointLight::getMaxT()
{
	return 1.0f;
}

PointLight::PointLightShader PointLight::getBuffer()
{
	PointLight::PointLightShader result;

	result.bright = this->getBright();
	result.maxT = this->getMaxT();
	result.shininess = this->getShininess();
	result.position = this->getPosition();

	return result;
}

VkDeviceSize PointLight::getBufferSize()
{
	return sizeof(PointLight::PointLightShader);
}

glm::vec4 PointLight::getPosition()
{
	return this->position;
}
