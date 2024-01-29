#pragma once
#include "../Utils/Vector/Vector3d.h"

class Light
{
public: 
	Light(const Vector3d const &position, float bright);

	float getBright();
	Vector3d getPosition();
	virtual Vector3d getLightVector(Vector3d &point) = 0;
	virtual bool hasPosition() = 0;
	virtual float getMaxT() = 0;
private:
	float bright = 0;
	Vector3d position;
};
