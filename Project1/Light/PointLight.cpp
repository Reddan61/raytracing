#include "PointLight.h"

PointLight::PointLight(const glm::vec3& const position, float bright): Light(position, bright)
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

PointLight::PointLightShader PointLight::getPointLightBuffer()
{
	PointLight::PointLightShader result;

	result.bright = this->getBright();
	result.maxT = this->getMaxT();
	result.position = glm::vec4(this->getPosition(), 1.0f);

	return result;
}

VkDeviceSize PointLight::getPointLightBufferSize()
{
	return sizeof(PointLight::PointLightShader);
}
