#pragma once
#include "../Config.h"

class Light
{
public: 
	Light(float bright, float shininess);

	float getBright();
	float getShininess();
	virtual float getMaxT() = 0;
private:
	float bright = 0;
	float shininess = 8.0f;
};
