#pragma once
#include "Light.h"
#include "../C99/structs.h"

class PointLight : public Light
{
public:
	PointLight(const Vector3d& const position, float bright);

	Vector3d getLightVector(Vector3d& point) override;
	bool hasPosition() override;
	float getMaxT() override;

	C99PointLight getC99();
};

