#pragma once
#include "../Object.h"

class Triangle : public Object
{
public:
	// std140
	struct TriangleShader {
		bool single_side;
		float specular, reflective;
		alignas(16) glm::vec4 A, B, C;
		glm::vec4 color;
	};

	Triangle(
		const glm::vec3 const & A, 
		const glm::vec3 const & B, 
		const glm::vec3 const & C, 
		const glm::vec3 const &color, 
		float specular = -1.0f, 
		float reflective = 0
	);
	~Triangle();

	glm::vec3 getNormal(const glm::vec3 const& point, const glm::vec3 const& direction) override;
	TriangleShader getShader();
	static VkDeviceSize getBufferSize();

	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
private:
	glm::vec3 A, B, C;
	bool isSingleSide = false;
};

