#pragma once
#include "../Utils/Color/Color.h"
#include "../Utils/Vector/Vector3d.h"
#include "../Math.h"

class Sky
{
public:
	virtual Color getColor(const Vector3d &direction);
};

