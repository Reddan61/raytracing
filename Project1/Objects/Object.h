#pragma once
#include "../Config.h"
#include "../Rotation/RotationMatrix.h"
#include "../Animation/Animation.h"

class Object
{
public:
	glm::vec3 getColor();
	float getSpecular();
	float getReflective();

	virtual glm::vec3 getNormal(const glm::vec3 const &point, const glm::vec3 const &direction) = 0;
	virtual void update(float delta) = 0;
	virtual void changePosition(const glm::vec3 const& position) = 0;
	void rotate(float xAngle, float yAngle);
	void setAnimation(Animation *animation);
protected: 
	bool is_updated = false;
	glm::vec3 color;
	float reflective = 0.f;
	float specular = -1;
	RotationMatrix* rotation_matrix = nullptr;
	Animation* animation = nullptr;
};

