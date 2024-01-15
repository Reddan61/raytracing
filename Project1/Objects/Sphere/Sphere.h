#pragma once
#include <iostream>
#include "../Object.h";
#include "../../Math.h";

class Sphere : public Object
{
public:
	Sphere(const sf::Vector3f& const position, const sf::Color& const color, int radius = 1, float specular = -1, float reflective = 0);
	
	sf::Vector3f getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction) override;

	Object::InsertRayValue insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) override;
	void update(sf::RenderWindow& window, sf::Time time) override;
	void changePosition(const sf::Vector3f const& position) override;
private:
	int radius;
	sf::Vector3f position;

	void render(sf::RenderWindow& window) override;
};

