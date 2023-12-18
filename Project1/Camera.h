#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

class Camera
{
public:
	Camera(float x, float y, float z);

	sf::Vector3f getPosition();

private:
	sf::Vector3f position;
	sf::CircleShape object;

	//void keyCheck(sf::Time time);
	//void changePosition(sf::Uint16 x, sf::Uint16 y);
	//void changeAngle(float angle);
};

