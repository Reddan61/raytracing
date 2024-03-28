#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const glm::vec3& const position, float bright);

	glm::vec3 getLightVector(glm::vec3& point) override;
	bool hasPosition() override;
	float getMaxT() override;
};

