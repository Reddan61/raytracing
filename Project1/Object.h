#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	const sf::Vector3f& getPosition();
	virtual void update(sf::RenderWindow &window, sf::Time time) = 0;
	virtual const sf::Vector2f& insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) = 0;
	virtual const sf::Color& getColor();

protected: 
	sf::Vector3f position;
	sf::Color color;
	virtual void render(sf::RenderWindow &window) = 0;
};

