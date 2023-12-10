#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h";

class Viewport
{
public:
	Viewport(sf::Uint16 w, sf::Uint16 h);
	~Viewport();

	Pixel** getPixels();
	void update(sf::RenderWindow& window);
	void updatePixel(sf::Uint16 x, sf::Uint16 y, const sf::Color &color);

	sf::Vector2f getSize();
private:
	sf::Uint16 w = 600;
	sf::Uint16 h = 600;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	Pixel **pixels;

	void updateTexture();
};

