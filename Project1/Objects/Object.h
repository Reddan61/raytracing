#pragma once
#include <SFML/Graphics.hpp>
#include "../Utils/Color/Color.h"
#include "../Utils/Vector/Vector3d.h"

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

	Color getColor();
	float getSpecular();
	float getReflective();

	virtual Vector3d getNormal(const Vector3d const &point, const Vector3d const &direction) = 0;
	virtual void update(sf::RenderWindow &window, sf::Time time) = 0;
	virtual Object::InsertRayValue insertRay(Vector3d& cameraPosition, Vector3d& direction) = 0;
	virtual void changePosition(const Vector3d const& position) = 0;
protected: 
	Color color;
	float reflective = 0.f;
	float specular = -1;
	virtual void render(sf::RenderWindow &window) = 0;
};

