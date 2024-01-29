#pragma once
#include<iostream>
#include "Utils/Color/Color.h"

class Pixel
{
public:
	Pixel();
	Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha);
	Pixel(Pixel &other);
	~Pixel();

	Color* getColor();
	void setColor(const Color& color);
private:
	Color *color = nullptr;
};

