#pragma once
#include "../Object.h"
#include "./../../Math.h"

class Triangle : public Object
{
public:
	Triangle(const sf::Vector3f const & A, const sf::Vector3f const & B, const sf::Vector3f const & C, const sf::Color const &color, float specular = -1, float reflective = 0);
	~Triangle();

	sf::Vector3f getNormal(const sf::Vector3f const& point, const sf::Vector3f const& direction) override;

	void update(sf::RenderWindow& window, sf::Time time) override;
	Object::InsertRayValue insertRay(sf::Vector3f& cameraPosition, sf::Vector3f& direction) override;
	void changePosition(const sf::Vector3f const& position) override;
private:
	sf::Vector3f A, B, C;
	bool isSingleSide = false;

	void render(sf::RenderWindow& window) override;
};

