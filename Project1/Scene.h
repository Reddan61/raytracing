#pragma once
#include <vector>
#include "Object.h"
#include "Camera.h"
#include "Light/Light.h"
#include "Light/AmbientLight.h"

class Scene
{
public:
	Scene(std::vector<Object*> *objects, std::vector<Light*> *lights, Camera *camera);
	~Scene();

	std::vector<Object*>* getSceneObjects();
	std::vector<Light*>* getSceneLights();
	Camera* getCamera();

	void update(sf::RenderWindow &window, sf::Time time);
private:
	std::vector<Object*> *SceneObjects = nullptr;
	std::vector<Light*> *SceneLights = nullptr;
	Camera* camera = nullptr;

	//void renderObjects(sf::RenderWindow &window, sf::Time time);
};

