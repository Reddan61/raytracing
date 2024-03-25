#include "Camera.h"

Camera::Camera(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);

	//this->rotationX = new Matrix4d();
	//this->rotationY = new Matrix4d();
	this->rotationX = glm::mat4x4(0);
	this->rotationY = glm::mat4x4(0);

	this->front = glm::vec3(0, 0, 1.0f);
	this->up = glm::vec3(0, 1.0f, 0);

	this->setRotationX(0);
	this->setRotationY(0);
	this->calculatedRotations();
}

Camera::~Camera()
{
	//delete this->rotationX;
	//delete this->rotationY;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

void Camera::update(GLFWwindow* window, float delta)
{
	this->keyCheck(window, delta);
}

glm::mat4 Camera::getRotation()
{
	return this->rotationY * this->rotationX;
}

Camera::CameraVulkan Camera::getBufferStruct()
{
	CameraVulkan result;

	result.position = this->position;
	result.rotation = this->getRotation();

	return result;
}

void Camera::keyCheck(GLFWwindow* window, float delta)
{
	bool moved = false;
	bool rotated = false;
	glm::vec3 right = glm::normalize(glm::cross(this->front, this->up));

	float move_speed = this->SPEEDMOVE * delta;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->position += this->front * move_speed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->position -= this->front * move_speed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->position -= right * move_speed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->position += right * move_speed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->position += this->up * move_speed;
		moved = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		this->position -= this->up * move_speed;
		moved = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		this->setRotationY(this->rotationYAngle + this->SPEEDSENS);
		rotated = true;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		this->setRotationY(this->rotationYAngle - this->SPEEDSENS);
		rotated = true;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->setRotationX(this->rotationXAngle + this->SPEEDSENS);
		rotated = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		this->setRotationX(this->rotationXAngle - this->SPEEDSENS);
		rotated = true;
	}	

	if (rotated) {
		this->calculatedRotations();
	}

}

void Camera::setRotationX(float angle)
{
	int result = angle;

	if (result > 89) {
		result = 89;
	}
	else if (result < -89) {
		result = -89;
	}

	this->rotationXAngle = angle;
	float radians = glm::radians(this->rotationXAngle);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(1.0f, 0.0f, 0.0f));

	this->rotationX = rotationMatrix;
}

void Camera::setRotationY(float angle)
{
	this->rotationYAngle = angle;

	float radians = glm::radians(this->rotationYAngle);

	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0.0f, 1.0f, 0.0f));

	this->rotationY = rotationMatrix;
}

void Camera::calculatedRotations()
{
	this->calculateFront();
	this->calculateUp();
}

void Camera::calculateFront()
{
	this->front = glm::normalize(this->getRotation() * glm::vec4(0, 0, -1.f, 1.0f));
}

void Camera::calculateUp()
{
	this->up = glm::normalize(this->getRotation() * glm::vec4(0, 1.f, 0, 1.0f));
}
