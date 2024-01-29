#pragma once
#include <iostream>
#include "../Object.h";
#include "../../Math.h";
#include "../../C99/structs.h"

class Sphere : public Object
{
public:
	Sphere(const Vector3d& const position, const Color& const color, int radius = 1, float specular = -1, float reflective = 0);
	
	Vector3d getNormal(const Vector3d const& point, const Vector3d const& direction) override;

	Object::InsertRayValue insertRay(Vector3d& cameraPosition, Vector3d& direction) override;
	void update(sf::RenderWindow& window, sf::Time time) override;
	void changePosition(const Vector3d const& position) override;
	Vector3d getPosition();

	C99Sphere getC99();

private:
	int radius;
	Vector3d position;

	void render(sf::RenderWindow& window) override;
};

