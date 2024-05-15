#include "RotationMatrix.h"

RotationMatrix::RotationMatrix(float xAngle, float yAngle)
{
	this->rotationX = glm::mat4x4(0);
	this->rotationY = glm::mat4x4(0);

	this->front = glm::vec3(0, 0, 1.0f);
	this->up = glm::vec3(0, 1.0f, 0);
	this->right = glm::normalize(glm::cross(this->front, this->up));

	this->setRotationX(xAngle);
	this->setRotationY(yAngle);
}

RotationMatrix::~RotationMatrix()
{
}

glm::mat4 RotationMatrix::getRotation()
{
	return this->rotation;
}

glm::vec3 RotationMatrix::getFront()
{
	return this->front;
}

glm::vec3 RotationMatrix::getUp()
{
	return this->up;
}

glm::vec3 RotationMatrix::getRight()
{
	return this->right;
}

float RotationMatrix::getRotationX()
{
	return this->rotationXAngle;
}

float RotationMatrix::getRotationY()
{
	return this->rotationYAngle;
}

void RotationMatrix::setRotationX(float angle)
{
	if (angle >= 89.0f) {
		angle = 89.0f;
	}
	else if (angle <= -89.0f) {
		angle = -89.0f;
	}

	this->rotationXAngle = angle;
	float radians = glm::radians(this->rotationXAngle);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(1.0f, 0.0f, 0.0f));

	this->rotationX = rotationMatrix;

	this->calculateRotation();
}

void RotationMatrix::setRotationY(float angle)
{
	this->rotationYAngle = angle;

	float radians = glm::radians(this->rotationYAngle);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));

	this->rotationY = rotationMatrix;

	this->calculateRotation();
}

void RotationMatrix::calculateRotation()
{
	this->rotation = this->rotationY * this->rotationX;
	this->front = glm::normalize(this->rotation * glm::vec4(0, 0, -1.f, 1.0f));
	this->up = glm::normalize(this->rotation * glm::vec4(0, 1.f, 0, 1.0f));
	this->right = glm::normalize(glm::cross(this->front, this->up));
}
