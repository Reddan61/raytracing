#pragma once
#include "Light.h"
#include "../Math.h"
#include "../C99/structs.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const Vector3d& const position, float bright);

	Vector3d getLightVector(Vector3d& point) override;
	bool hasPosition() override;
	float getMaxT() override;

	C99DirectionalLight getC99();
};

