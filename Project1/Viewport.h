#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h";

class Viewport
{
public:
	Viewport(unsigned int w, unsigned int h);
	~Viewport();

	Pixel*** getPixels();
	void update(sf::RenderWindow& window);
	void updatePixel(unsigned int x, unsigned int y, const Color &color);

	sf::Vector2<int> getSize();
private:
	unsigned int w, h;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	Pixel ***pixels = nullptr;

	void updateTexture();
};

