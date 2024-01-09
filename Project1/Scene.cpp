#include "Scene.h"

Scene::Scene(std::vector<Object*> *objects, std::vector<Light*> *lights, Camera *camera)
{
	this->camera = camera;
	this->SceneObjects = objects;
	this->SceneLights = lights;
	this->sky = new Sky();
}

Scene::~Scene()
{
	delete this->camera;
	delete this->SceneObjects;
	delete this->SceneLights;
	delete this->sky;
}

void Scene::update(sf::RenderWindow &window, sf::Time time)
{
	this->getCamera()->update(window, time);
	//this->renderObjects(window, time);
}

std::vector<Object*>* Scene::getSceneObjects()
{
	return this->SceneObjects;
}

std::vector<Light*>* Scene::getSceneLights()
{
	return this->SceneLights;
}

Camera* Scene::getCamera()
{
	return this->camera;
}

Sky* Scene::getSky()
{
	return this->sky;
}

//void Scene::renderObjects(sf::RenderWindow &window, sf::Time time)
//{
//	for (Object* object : *this->SceneObjects) {
//		object->update(window, time);
//	}
//}
