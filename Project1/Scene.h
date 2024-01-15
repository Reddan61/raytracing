#pragma once
#include <vector>
#include "./Objects/Object.h"
#include "Camera.h"
#include "Light/Light.h"
#include "Light/AmbientLight.h"
#include "Sky/Sky.h"

class Scene
{
public:
	Scene(std::vector<Object*> *objects, std::vector<Light*> *lights, Camera *camera);
	~Scene();

	std::vector<Object*>* getSceneObjects();
	std::vector<Light*>* getSceneLights();
	Camera* getCamera();
	Sky* getSky();

	void update(sf::RenderWindow &window, sf::Time time);
private:
	std::vector<Object*> *SceneObjects = nullptr;
	std::vector<Light*> *SceneLights = nullptr;
	Camera* camera = nullptr;
	Sky* sky = nullptr;

	//void renderObjects(sf::RenderWindow &window, sf::Time time);
};

