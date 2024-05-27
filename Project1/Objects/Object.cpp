#include "Object.h"

glm::vec3 Object::getColor()
{
	return this->color;
}

float Object::getSpecular()
{
	return this->specular;
}

float Object::getReflective()
{
	return this->reflective;
}

void Object::rotate(float xAngle, float yAngle)
{
	this->rotation_matrix->setRotationX(this->rotation_matrix->getRotationX() + yAngle);
	this->rotation_matrix->setRotationY(this->rotation_matrix->getRotationY() + xAngle);
}

void Object::setAnimation(Animation* animation)
{
	this->animation = animation;
	this->animation->start();
}

