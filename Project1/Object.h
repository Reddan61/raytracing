#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	sf::Vector3f getPosition();
	sf::Color getColor();
	float getSpecular();
	float getReflective();

	virtual void update(sf::RenderWindow &window, sf::Time time) = 0;
	virtual sf::Vector2f insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) = 0;

protected: 
	sf::Vector3f position;
	sf::Color color;
	float reflective = 0.f;
	float specular = -1;
	virtual void render(sf::RenderWindow &window) = 0;
};

