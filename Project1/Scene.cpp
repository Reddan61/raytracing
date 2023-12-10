#include "Scene.h"

Scene::Scene(std::vector<Object*> *objects, Camera *camera)
{
	this->SceneObjects = objects;
	this->camera = camera;
}

Scene::~Scene()
{
	delete this->SceneObjects;
	delete this->camera;
}

void Scene::update(sf::RenderWindow &window, sf::Time time)
{
	this->renderObjects(window, time);
}

std::vector<Object*>* Scene::getSceneObjects()
{
	return this->SceneObjects;
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
