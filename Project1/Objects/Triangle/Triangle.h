#pragma once
#include "../Object.h"
#include "./../../Math.h"

class Triangle : public Object
{
public:
	Triangle(const Vector3d const & A, const Vector3d const & B, const Vector3d const & C, const Color const &color, float specular = -1, float reflective = 0);
	~Triangle();

	Vector3d getNormal(const Vector3d const& point, const Vector3d const& direction) override;

	void update(sf::RenderWindow& window, sf::Time time) override;
	Object::InsertRayValue insertRay(Vector3d& cameraPosition, Vector3d& direction) override;
	void changePosition(const Vector3d const& position) override;
private:
	Vector3d A, B, C;
	bool isSingleSide = false;

	void render(sf::RenderWindow& window) override;
};

