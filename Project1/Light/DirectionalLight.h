#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	// std140
	struct DirectionalLightShader {
		float bright, shininess;
		alignas(16) glm::vec4 direction;
	};

	DirectionalLight(const glm::vec3& const direction, float bright, float shininess);

	glm::vec3 getLightVector(glm::vec3& point) override;
	bool hasPosition() override;
	float getMaxT() override;

	DirectionalLightShader getBuffer();
private:
	glm::vec3 direction;
};

