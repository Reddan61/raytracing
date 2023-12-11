#pragma once
#include <SFML/Graphics.hpp>

class Light
{
public:
	Light(float bright);

	virtual sf::Vector3f getLightVector(sf::Vector3f &point) = 0;
	float getBright();
private:
	float bright = 0;
};
