#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	const sf::Vector3f& getPosition();
	const sf::Color& getColor();
	float getSpecular();

	virtual void update(sf::RenderWindow &window, sf::Time time) = 0;
	virtual const sf::Vector2f& insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) = 0;

protected: 
	sf::Vector3f position;
	sf::Color color;
	float specular = -1;
	virtual void render(sf::RenderWindow &window) = 0;
};

