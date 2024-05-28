#pragma once
#include <iostream>

#include "../Object.h";

class Sphere : public Object
{
public:
	// std140
	struct SphereShader {
		float radius, specular, reflective;
		alignas(16) glm::vec4 center;
		glm::vec4 color;
	};
	Sphere(const glm::vec4& const position, const glm::vec4& const color, float radius = 1.0f, float specular = -1.0f, float reflective = 0);

	void update(float delta) override;
	void changePosition(const glm::vec4 const& position) override;
	glm::vec4 getPosition();

	SphereShader getShader();
	static VkDeviceSize getBufferSize();

	bool isUpdated();
private:
	float radius;
	glm::vec4 position;
};

