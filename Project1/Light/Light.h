#pragma once
#include "../Config.h"

class Light
{
public: 
	Light(const glm::vec3 const &position, float bright);

	float getBright();
	glm::vec3 getPosition();
	virtual glm::vec3 getLightVector(glm::vec3 &point) = 0;
	virtual bool hasPosition() = 0;
	virtual float getMaxT() = 0;
private:
	float bright = 0;
	glm::vec3 position;
};
