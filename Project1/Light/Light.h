#pragma once
#include <SFML/Graphics.hpp>

class Light
{
public: 
	Light(const sf::Vector3f const &position, float bright);

	float getBright();
	sf::Vector3f getPosition();
	virtual sf::Vector3f getLightVector(sf::Vector3f &point) = 0;
	virtual bool hasPosition() = 0;
private:
	float bright = 0;
	sf::Vector3f position;
};
