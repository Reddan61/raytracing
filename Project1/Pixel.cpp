#include "Pixel.h"

Pixel::Pixel()
{
	this->color = new Color(0, 0, 0, 0);
}

Pixel::Pixel(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->color = new Color(r, g, b, alpha);
}

Pixel::Pixel(Pixel& other)
{
	Color *color = other.getColor();

	this->color = new Color(color->r, color->g, color->b, color->alpha);
}

Pixel::~Pixel()
{
	if (this->color == nullptr) return;

	delete this->color;
	this->color = nullptr;
}

Color* Pixel::getColor()
{
	return this->color;
}

void Pixel::setColor(const Color &color)
{
	this->color->r = color.r;
	this->color->g = color.g;
	this->color->b = color.b;
	this->color->alpha = color.alpha;
}
