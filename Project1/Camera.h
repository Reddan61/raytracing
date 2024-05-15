#pragma once
#include "./Rotation/RotationMatrix.h"
#include "./Objects/Object.h"

class Camera
{
public:
	struct CameraVulkan {
		glm::vec3 position;
		alignas(16) glm::mat4 rotation;
	};

	Camera(float x, float y, float z);
	~Camera();

	void update(GLFWwindow* window, float delta);
	void keyCheck(GLFWwindow* window, float delta);
	glm::vec3 getPosition();

	CameraVulkan getBufferStruct();
private:
	glm::vec3 position;
	RotationMatrix* rotation_matrix = nullptr;

	const float SPEEDMOVE = 2;
	const float SPEEDSENS = 1.0f;
};

