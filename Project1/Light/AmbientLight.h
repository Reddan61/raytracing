#pragma once
#include "Light.h"
#include "../Math.h"

class AmbientLight : public Light
{
public:
	AmbientLight(float bright);

	sf::Vector3f getLightVector(sf::Vector3f& point) override;
	bool hasPosition() override;
};

