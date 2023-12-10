#pragma once
#include <iostream>
#include "Object.h";
#include "Math.h";

class Sphere : public Object
{
public:
	Sphere(const sf::Vector3f& const position, const sf::Color& const color);
	void update(sf::RenderWindow& window, sf::Time time) override;

private:
	const int radius = 1;

	const sf::Vector2f& insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) override;
	void render(sf::RenderWindow& window) override;
};

