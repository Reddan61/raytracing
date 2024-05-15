#include "Camera.h"

Camera::Camera(float x, float y, float z) {
	this->position = glm::vec3(x, y, z);
	this->rotation_matrix = new RotationMatrix(0.0f, 0.0f);
}

Camera::~Camera()
{
	delete this->rotation_matrix;
}

glm::vec3 Camera::getPosition()
{
	return this->position;
}

void Camera::update(GLFWwindow* window, float delta)
{
	this->keyCheck(window, delta);
}

Camera::CameraVulkan Camera::getBufferStruct()
{
	CameraVulkan result;

	result.position = this->position;
	result.rotation = this->rotation_matrix->getRotation();

	return result;
}

void Camera::keyCheck(GLFWwindow* window, float delta)
{
	float move_speed = this->SPEEDMOVE * delta;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		this->position += this->rotation_matrix->getFront() * move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		this->position -= this->rotation_matrix->getFront() * move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		this->position -= this->rotation_matrix->getRight() * move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		this->position += this->rotation_matrix->getRight() * move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		this->position += this->rotation_matrix->getUp() * move_speed;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		this->position -= this->rotation_matrix->getUp() * move_speed;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		this->rotation_matrix->setRotationY(this->rotation_matrix->getRotationY() + this->SPEEDSENS);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		this->rotation_matrix->setRotationY(this->rotation_matrix->getRotationY() - this->SPEEDSENS);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		this->rotation_matrix->setRotationX(this->rotation_matrix->getRotationX() + this->SPEEDSENS);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		this->rotation_matrix->setRotationX(this->rotation_matrix->getRotationX() - this->SPEEDSENS);
	}	
}