#pragma once
#include <iostream>
#include <thread>
#include "Scene.h";
#include "Pixel.h";
#include "Viewport.h";
#include "Math.h";
#include "ThreadPool/ThreadPool.h"
#include <future>
#include "./Matrix/Matrix4d.h"

class Render
{
public:
	Render(int width, int height);
	Render(Render &other);
	~Render();
	
	void update(Scene *scene, sf::RenderWindow& window, sf::Time &time, ThreadPool &pool);
private:

	Viewport *viewport = nullptr;
	const int reflection_depth = 3;

	void calculate(Scene* scene, ThreadPool& pool);
	sf::Vector3f calculateDirection(float x, float y);
	sf::Color traceRay(Scene* scene, sf::Vector3f &cameraPosition, sf::Vector3f &direction, float min_t, float max_t, int reflection_depth, const Object const* origin = nullptr);
	float ComputeLighting(Scene* scene, sf::Vector3f &point, sf::Vector3f &normal, sf::Vector3f &view, float specular, const Object const* origin = nullptr);
	Object::InsertRayValue getClosesetObject(Scene* scene, sf::Vector3f& cameraPosition, sf::Vector3f& direction, float min_t, float max_t, const Object const* origin = nullptr);
	sf::Vector3f getReflectRay(const sf::Vector3f & const v1, const sf::Vector3f & const v2);

	sf::Color getPixelColor(float x, float y, Scene* scene, Matrix4d& rotation, sf::Vector3f& cameraPosition);
	void perPixel(float x, float y, Scene* scene, Matrix4d& rotation, sf::Vector3f& cameraPosition);
};

