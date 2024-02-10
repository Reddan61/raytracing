#pragma once
#include <iostream>
#include <thread>
#include "Scene.h";
#include "Pixel.h";
#include "Viewport.h";
#include "Math.h";
#include "ThreadPool/ThreadPool.h"
#include <future>
#include "./Utils/Matrix/Matrix4d.h"

class Render
{
public:
	Render(int width, int height);
	Render(Render &other);
	~Render();
	
	void update(Scene *scene, sf::RenderWindow& window, sf::Time &time, ThreadPool &pool);
	Color perPixel(float x, float y, Scene* scene, Matrix4d& rotation, Vector3d& cameraPosition);
private:
	Viewport *viewport = nullptr;
	const int reflection_depth = 3;

	void calculate(Scene* scene, ThreadPool& pool);
	Vector3d calculateDirection(float x, float y);
	Color traceRay(Scene* scene, Vector3d &cameraPosition, Vector3d &direction, float min_t, float max_t, int reflection_depth, const Object const* origin = nullptr);
	float ComputeLighting(Scene* scene, Vector3d &point, Vector3d &normal, Vector3d &view, float specular, const Object const* origin = nullptr);
	Object::InsertRayValue getClosesetObject(Scene* scene, Vector3d& cameraPosition, Vector3d& direction, float min_t, float max_t, const Object const* origin = nullptr);
	Vector3d getReflectRay(const Vector3d & const v1, const Vector3d & const v2);

	Color getPixelColor(float x, float y, Scene* scene, Matrix4d& rotation, Vector3d& cameraPosition);
};

