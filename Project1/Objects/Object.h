#pragma once
#include "../Config.h"

class Object
{
public:
	glm::vec3 getColor();
	float getSpecular();
	float getReflective();

	virtual glm::vec3 getNormal(const glm::vec3 const &point, const glm::vec3 const &direction) = 0;
	virtual void update(GLFWwindow* window, float delta) = 0;
	virtual void changePosition(const glm::vec3 const& position) = 0;
protected: 
	glm::vec3 color;
	float reflective = 0.f;
	float specular = -1;
};

