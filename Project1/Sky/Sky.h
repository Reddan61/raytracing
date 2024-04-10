#pragma once
#include "../Config.h"

class Sky
{
public:
	virtual glm::vec3 getColor(const glm::vec3&direction);
};

