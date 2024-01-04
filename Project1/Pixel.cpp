#include "Pixel.h"

Pixel::Pixel()
{
	this->color = new sf::Color(0, 0, 0, 0);
}

Pixel::Pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 alpha)
{
	this->color = new sf::Color(r, g, b, alpha);
}

Pixel::Pixel(Pixel& other)
{
	sf::Color *color = other.getColor();

	this->color = new sf::Color(color->r, color->g, color->b, color->a);
}

Pixel::~Pixel()
{
	if (this->color == nullptr) return;

	delete this->color;
	this->color = nullptr;
}

sf::Color* Pixel::getColor()
{
	return this->color;
}

void Pixel::setColor(const sf::Color &color)
{
	this->color->r = color.r;
	this->color->g = color.g;
	this->color->b = color.b;
	this->color->a = color.a;
}
