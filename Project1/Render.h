#pragma once
#include <iostream>
#include "Scene.h";
#include "Pixel.h";
#include "Viewport.h";
#include "Math.h";

class Render
{
public:
	Render(int width, int height);
	~Render();
	
	void update(Scene *scene, sf::RenderWindow& window, sf::Time time);
private:
	Viewport *viewport = nullptr;
	void calculate(Scene* scene);
	const sf::Vector3f & calculateDirection(int x, int y);
	const sf::Color& traceRay(Scene* scene, sf::Vector3f &cameraPosition, sf::Vector3f &direction, int min_t, int max_t);
};

