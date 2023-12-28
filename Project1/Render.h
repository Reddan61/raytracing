#pragma once
#include <iostream>
#include "Scene.h";
#include "Pixel.h";
#include "Viewport.h";
#include "Math.h";
#include "./Matrix/Matrix4d.h"

class Render
{
public:
	Render(int width, int height);
	~Render();
	
	void update(Scene *scene, sf::RenderWindow& window, sf::Time time);
private:
	Viewport *viewport = nullptr;
	const int reflection_depth = 3;

	void calculate(Scene* scene);
	sf::Vector3f calculateDirection(int x, int y);
	sf::Color traceRay(Scene* scene, sf::Vector3f &cameraPosition, sf::Vector3f &direction, float min_t, float max_t, int reflection_depth, Object* origin = nullptr);
	float ComputeLighting(Scene* scene, sf::Vector3f &point, sf::Vector3f &normal, sf::Vector3f &view, float specular);
	std::pair<Object*, float> getClosesetObject(Scene* scene, sf::Vector3f& cameraPosition, sf::Vector3f& direction, float min_t, float max_t, Object* origin = nullptr);
	sf::Vector3f getReflectRay(const sf::Vector3f & const v1, const sf::Vector3f & const v2);
};

