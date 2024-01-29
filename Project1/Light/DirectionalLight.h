#pragma once
#include "Light.h"
#include "../Math.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector3d& const position, float bright);

	Vector3d getLightVector(Vector3d& point) override;
	bool hasPosition() override;
	float getMaxT() override;
};

