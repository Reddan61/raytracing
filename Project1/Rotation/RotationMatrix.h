#pragma once
#include "../Config.h"

class RotationMatrix
{
public:
	RotationMatrix(float xAngle, float yAngle);
	~RotationMatrix();

	glm::mat4 getRotation();
	glm::vec3 getFront();
	glm::vec3 getUp();
	glm::vec3 getRight();

	float getRotationX();
	float getRotationY();

	void setRotationX(float angle);
	void setRotationY(float angle);

	void update(GLFWwindow* window, float delta);
private:
	void calculateRotation();

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;

	float rotationXAngle = 0;
	float rotationYAngle = 0;

	glm::mat4 rotationY;
	glm::mat4 rotationX;
	glm::mat4 rotation;
};

