#pragma once
#include "Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(float bright);

	glm::vec3 getLightVector(glm::vec3& point) override;
	bool hasPosition() override;
	float getMaxT() override;
};

