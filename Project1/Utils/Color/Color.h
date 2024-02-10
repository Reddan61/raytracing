#pragma once
#include "../Vector/Vector3d.h"
#include "../../C99/structs.h"

class Vector3d;

class Color
{
public:
	Color();
	Color(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha);
	Color(const Vector3d& vec);
	Color(C99Color col);
	~Color();

	C99Color getC99();

	Color operator +(const Color const& other) const;

	Color operator /(const float num) const;

	Color& operator =(const Color& other);

	unsigned int r, g, b = 0;
	int alpha = 255;
};

Color operator *(const float left, const Color& right);
