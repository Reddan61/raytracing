#pragma once
#include <vector>
#include "Object.h"
#include "Camera.h"

class Scene
{
public:
	Scene(std::vector<Object*> *objects, Camera *camera);
	~Scene();

	std::vector<Object*>* getSceneObjects();
	Camera*& getCamera();

	void update(sf::RenderWindow &window, sf::Time time);
private:
	std::vector<Object*> *SceneObjects;
	Camera* camera = nullptr;

	void renderObjects(sf::RenderWindow &window, sf::Time time);
};

