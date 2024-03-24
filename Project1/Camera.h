#pragma once
#include <SFML/Graphics.hpp>
#include "./Objects/Object.h"
#include "Math.h"
#include "./Utils/Matrix/Matrix3d.h"
#include "./Utils/Vector/Vector3d.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	struct CameraVulkan {
		glm::vec3 position;
		alignas(16) glm::mat4 rotation;
	};

	Camera(float x, float y, float z);
	~Camera();

	void update(double deltaTime);
	void keyCheck(GLFWwindow* window, float delta);
	glm::vec3 getPosition();
	glm::mat4 getRotation();

	CameraVulkan getBufferStruct();
private:
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;

	const float SPEEDMOVE = 2;
	const float SPEEDSENS = 0.5;
	float rotationXAngle = 0;
	float rotationYAngle = 0;

	glm::mat4 rotationY;
	glm::mat4 rotationX;

	void setRotationX(float angle);
	void setRotationY(float angle);

	void calculatedRotations();
	void calculateFront();
	void calculateUp();
};

