#pragma once
#include<iostream>
#include <SFML/Graphics.hpp>

class Pixel
{
public:
	Pixel();
	Pixel(sf::Uint8 r, sf::Uint8 g, sf::Uint8 b, sf::Uint8 alpha);
	Pixel(Pixel &other);
	~Pixel();

	sf::Color* getColor();
	void setColor(const sf::Color& color);
private:
	sf::Color *color = nullptr;
};

