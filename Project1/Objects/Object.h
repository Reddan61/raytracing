#pragma once
#include <SFML/Graphics.hpp>

class Object
{
public:
	struct InsertRayValue {
		InsertRayValue(float t1, float t2, Object *object = nullptr) {
			this->t1 = t1;
			this->t2 = t2;
			this->object = object;
		}

		InsertRayValue(const InsertRayValue const& other) {
			this->t1 = other.t1;
			this->t2 = other.t2;
			this->object = other.object;
		}

		float t1, t2 = 0;
		Object* object = nullptr;
	};

	sf::Color getColor();
	float getSpecular();
	float getReflective();

	virtual sf::Vector3f getNormal(const sf::Vector3f const &point, const sf::Vector3f const &direction) = 0;
	virtual void update(sf::RenderWindow &window, sf::Time time) = 0;
	virtual Object::InsertRayValue insertRay(sf::Vector3f & cameraPosition, sf::Vector3f & direction) = 0;
	virtual void changePosition(const sf::Vector3f const& position) = 0;
protected: 
	sf::Color color;
	float reflective = 0.f;
	float specular = -1;
	virtual void render(sf::RenderWindow &window) = 0;
};

