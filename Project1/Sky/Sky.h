#pragma once
#include <SFML/Graphics.hpp>
#include "../Math.h"

class Sky
{
public:
	virtual sf::Color getColor(const sf::Vector3f &direction);
};

