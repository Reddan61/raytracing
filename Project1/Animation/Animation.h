#pragma once
#include <iostream>
#include "../Config.h"

class Animation
{
public:
	Animation(glm::vec3 start, glm::vec3 end, float speed, bool circled = false);
	~Animation();

	void start();
	void stop();

	glm::vec3 getPosition();

	bool update(float delta);
private:
	bool is_stop = true;
	bool is_updated = false;
	bool circled = false;
	float speed = 0.0f;
	glm::vec3 _start, _end, _current;
};

