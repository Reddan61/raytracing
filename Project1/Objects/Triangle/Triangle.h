#pragma once
#include "../Object.h"

class Triangle : public Object
{
public:
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

	void update(GLFWwindow* window, float delta) override;
	void changePosition(const glm::vec3 const& position) override;
private:
	glm::vec3 A, B, C;
	bool isSingleSide = false;
};

