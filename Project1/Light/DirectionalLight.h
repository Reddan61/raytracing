#pragma once
#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight(const sf::Vector3f& const position, float bright);

	sf::Vector3f getLightVector(sf::Vector3f& point) override;
	bool hasPosition() override;
};

