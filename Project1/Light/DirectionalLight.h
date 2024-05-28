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

	DirectionalLight(const glm::vec4& const direction, float bright, float shininess);

	glm::vec4 getDirection();
	glm::vec4 getLightVector(glm::vec4& point);
	float getMaxT() override;

	DirectionalLightShader getBuffer();
private:
	glm::vec4 direction;
};

