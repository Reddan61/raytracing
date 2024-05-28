#pragma once
#include "Light.h"

class AmbientLight : public Light
{
public:
	AmbientLight(float bright);

	float getMaxT() override;
};

