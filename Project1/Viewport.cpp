#include "Viewport.h"


Viewport::Viewport(sf::Uint16 w, sf::Uint16 h)
{
	this->w = w;
	this->h = h;

	this->image.create(this->w, this->h, sf::Color(0, 0, 0, 0));
	this->texture.create(this->w, this->h);

	this->pixels = new Pixel *[this->w];

	for (sf::Uint16 y = 0; y < this->h; y++) {
		this->pixels[y] = new Pixel[this->h];
		
		for (sf::Uint16 x = 0; x < this->w; x++) {
			this->pixels[y][x].setColor(sf::Color(255, 0, 0, 255));
			this->image.setPixel(x, y, sf::Color(255, 0, 0, 255));
		}
	}

	this->texture.update(this->image);
	this->sprite.setTexture(this->texture);
}

Viewport::~Viewport()
{
	for (sf::Uint16 x = 0; x < this->w; x++) {
		delete[] this->pixels[x];
	}

	delete[] this->pixels;
}

Pixel** Viewport::getPixels()
{
	return this->pixels;
}

void Viewport::update(sf::RenderWindow& window)
{
	this->updateTexture();

	window.draw(this->sprite);
}

void Viewport::updatePixel(sf::Uint16 x, sf::Uint16 y,	const sf::Color &color)
{
	this->pixels[y][x].setColor(color);
	this->image.setPixel(x, y, color);
}

sf::Vector2<int> Viewport::getSize()
{
	return sf::Vector2<int>(this->w, this->h);
}

void Viewport::updateTexture()
{
	this->texture.update(this->image);
}
