#pragma once
#include "Light.h"
#include "../Math.h"
#include "../C99/structs.h"

class AmbientLight : public Light
{
public:
	AmbientLight(float bright);

	Vector3d getLightVector(Vector3d& point) override;
	bool hasPosition() override;
	float getMaxT() override;

	C99AmbientLight* getC99();
};

