#include "PointLight.h"

PointLight::PointLight(const glm::vec3& const position, float bright, float shininess): Light(position, bright, shininess)
{
}

glm::vec3 PointLight::getLightVector(glm::vec3& point)
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

PointLight::PointLightShader PointLight::getBuffer()
{
	PointLight::PointLightShader result;

	result.bright = this->getBright();
	result.maxT = this->getMaxT();
	result.shininess = this->getShininess();
	result.position = glm::vec4(this->getPosition(), 1.0f);

	return result;
}

VkDeviceSize PointLight::getBufferSize()
{
	return sizeof(PointLight::PointLightShader);
}
