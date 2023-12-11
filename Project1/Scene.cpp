#include "Scene.h"

Scene::Scene(std::vector<Object*> *objects, std::vector<Light*> *lights, Camera *camera)
{
	this->camera = camera;
	this->SceneObjects = objects;
	this->SceneLights = lights;
	this->SceneAmbientLight = new AmbientLight(0.2f);
}

Scene::~Scene()
{
	delete this->camera;
	delete this->SceneObjects;
	delete this->SceneLights;
	delete this->SceneAmbientLight;
}

void Scene::update(sf::RenderWindow &window, sf::Time time)
{
	this->renderObjects(window, time);
}

std::vector<Object*>* Scene::getSceneObjects()
{
	return this->SceneObjects;
}

std::vector<Light*>* Scene::getSceneLights()
{
	return this->SceneLights;
}

AmbientLight* Scene::getSceneAmbientLight()
{
	return this->SceneAmbientLight;
}

Camera*& Scene::getCamera()
{
	return this->camera;
}

void Scene::renderObjects(sf::RenderWindow &window, sf::Time time)
{
	for (Object* object : *this->SceneObjects) {
		object->update(window, time);
	}
}
