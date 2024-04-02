#pragma once
#include <iostream>

#include "../Object.h";
#include "../../Math.h";

class Sphere : public Object
{
public:
	// std140
	struct SphereShader {
		float radius, specular, reflective;
		alignas(16) glm::vec4 center;
		glm::vec4 color;
	};
	Sphere(const glm::vec3& const position, const glm::vec3& const color, float radius = 1.0f, float specular = -1.0f, float reflective = 0);

	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
	glm::vec3 getPosition();
	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;

	SphereShader getShader();
	static VkDeviceSize getBufferSize();
private:
	float radius;
	glm::vec3 position;
};

