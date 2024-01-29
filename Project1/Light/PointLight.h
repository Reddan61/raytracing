#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const Vector3d& const position, float bright);

	Vector3d getLightVector(Vector3d& point) override;
	bool hasPosition() override;
	float getMaxT() override;
};

