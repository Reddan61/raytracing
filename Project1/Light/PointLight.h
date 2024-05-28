#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	// std140
	struct PointLightShader {
		float bright, maxT, shininess;
		alignas(16) glm::vec4 position;
	};

	PointLight(const glm::vec4& const position, float bright, float shininess);

	glm::vec4 getLightVector(glm::vec4& point);
	float getMaxT() override;
	PointLightShader getBuffer();
	static VkDeviceSize getBufferSize();
	glm::vec4 getPosition();
private:
	glm::vec4 position;
};

