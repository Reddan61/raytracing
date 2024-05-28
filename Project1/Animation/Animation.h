#pragma once
#include <iostream>
#include "../Config.h"

class Animation
{
public:
	Animation(glm::vec4 start, glm::vec4 end, float speed, bool circled = false);
	~Animation();

	void start();
	void stop();

	glm::vec4 getPosition();

	bool update(float delta);
private:
	bool is_stop = true;
	bool is_updated = false;
	bool circled = false;
	float speed = 0.0f;
	glm::vec4 _start, _end, _current;
};

