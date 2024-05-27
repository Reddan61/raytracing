#include "Animation.h"

Animation::Animation(glm::vec3 start, glm::vec3 end, float speed, bool circled)
{
	this->_start = start;
	this->_current = start;
	this->_end = end;
	this->speed = speed;

	this->is_stop = true;
	this->is_updated = false;
	this->circled = circled;
}

Animation::~Animation()
{
}

void Animation::start()
{
	this->is_stop = false;
}

void Animation::stop()
{
	this->is_stop = true;
}

glm::vec3 Animation::getPosition()
{
	return this->_current;
}

bool Animation::update(float delta)
{
	if (this->is_stop == true) {
		return false;
	} 

	glm::vec3 direction = glm::normalize(this->_end - this->_current);
	float new_speed = this->speed * delta;
	float distance = glm::distance(this->_current, this->_end);

	if (distance > new_speed) {
		glm::vec3 new_current = this->_current + (direction * new_speed);
		this->_current = new_current;
	}
	else {
		if (this->circled) {
			glm::vec3 temp = this->_start;
			this->_start = this->_end;
			this->_end = temp;
			this->_current = this->_start;
		}
		else {
			this->_current = this->_end;
			this->stop();
		}
	}

	return true;
}
