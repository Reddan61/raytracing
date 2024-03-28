#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	// std140
	struct PointLightShader {
		float bright, maxT;
		alignas(16) glm::vec4 position;
	};

	PointLight(const glm::vec3& const position, float bright);

	glm::vec3 getLightVector(glm::vec3& point) override;
	bool hasPosition() override;
	float getMaxT() override;
	PointLightShader getPointLightBuffer();
	static VkDeviceSize getPointLightBufferSize();
};

