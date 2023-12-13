#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight(const sf::Vector3f& const position, float bright);

	sf::Vector3f getLightVector(sf::Vector3f& point) override;
	bool hasPosition() override;
	float getMaxT() override;
};

