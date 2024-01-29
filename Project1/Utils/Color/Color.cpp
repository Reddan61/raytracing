#include "Color.h"

Color::Color()
{
	this->r = 0;
	this->g = 0;
	this->b = 0;
	this->alpha = 255;
}

Color::Color(unsigned int r, unsigned int g, unsigned int b, unsigned int alpha)
{
	this->r = r > 255 ? 255 : r;
	this->g = g > 255 ? 255 : g;
	this->b = b > 255 ? 255 : b;
	this->alpha = alpha > 255 ? 255 : alpha;
}

Color::Color(const Vector3d& vec)
{
	this->r = vec.x > 255 ? 255 : vec.x;
	this->g = vec.y > 255 ? 255 : vec.y;
	this->b = vec.z > 255 ? 255 : vec.z;
	this->alpha = 255;
}

Color::~Color()
{
}

C99Color Color::getC99()
{
	C99Color result;

	result.r = this->r;
	result.g = this->g;
	result.b = this->b;

	return result;
}

Color Color::operator+(const Color const& other) const
{
	return Color(
		this->r + other.r, 
		this->g + other.g, 
		this->b + other.b, 
		255
	);
}

Color Color::operator /(const float num) const
{
	return Color(
		this->r / num,
		this->g / num,
		this->b / num,
		255
	);
}

Color& Color::operator=(const Color& other)
{
	if (&other != this) {
		this->r = other.r;
		this->g = other.g;
		this->b = other.b;
		this->alpha = other.alpha;
	}

	return *this;
}

Color operator*(const float left, const Color& right)
{
	return Color(
		left * right.r,
		left * right.g,
		left * right.b,
		right.alpha
	);
}
