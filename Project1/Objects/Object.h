#pragma once
#include "../Config.h"
#include "../Rotation/RotationMatrix.h"
#include "../Animation/Animation.h"

class Object
{
public:
	glm::vec4 getColor();
	float getSpecular();
	float getReflective();

	virtual void update(float delta) = 0;
	virtual void changePosition(const glm::vec4 const& position) = 0;
	void rotate(float xAngle, float yAngle);
	void setAnimation(Animation *animation);
protected: 
	bool is_updated = false;
	glm::vec4 color;
	float reflective = 0.f;
	float specular = -1;
	RotationMatrix* rotation_matrix = nullptr;
	Animation* animation = nullptr;
};

