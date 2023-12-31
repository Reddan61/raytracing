#pragma once
#include <SFML/Graphics.hpp>
#include "Pixel.h";

class Viewport
{
public:
	Viewport(sf::Uint16 w, sf::Uint16 h);
	~Viewport();

	//Pixel*** getPixels();
	void update(sf::RenderWindow& window);
	void updatePixel(sf::Uint16 x, sf::Uint16 y, const sf::Color &color);

	sf::Vector2<int> getSize();
private:
	sf::Uint16 w;
	sf::Uint16 h;

	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;

	Pixel ***pixels = nullptr;

	void updateTexture();
};

