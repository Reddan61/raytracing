#include "Scene.h"

Scene::Scene(Camera *camera)
{
	this->SceneObjects = new std::vector<std::shared_ptr<Object>>;
	this->SceneLights = new std::vector<Light*>;
	this->sky = new Sky();

	this->spheres = new std::vector<std::shared_ptr<Sphere>>;
	this->triangles = new std::vector<std::shared_ptr<Triangle>>;
	this->meshes = new std::vector<std::shared_ptr<TriangleMesh>>;

	this->camera = camera;
}

Scene::~Scene()
{
	delete this->camera;
	delete this->SceneObjects;
	delete this->SceneLights;
	delete this->spheres;
	delete this->triangles;
	delete this->meshes;
	delete this->sky;
}

void Scene::addSphere(Sphere* sphere)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(sphere));
	this->spheres->push_back(std::shared_ptr<Sphere>(sphere));
}

void Scene::addTriangle(Triangle* triangle)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(triangle));
	this->triangles->push_back(std::shared_ptr<Triangle>(triangle));
}

void Scene::addMesh(TriangleMesh* mesh)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(mesh));
	this->meshes->push_back(std::shared_ptr<TriangleMesh>(mesh));
}

void Scene::addLight(Light* light)
{
	this->SceneLights->push_back(light);
}

C99Scene Scene::getGpuScene()
{
	C99Scene result;

	const int size = this->spheres->size();

	C99Sphere* spheres = new C99Sphere[size];

	for (int i = 0; i < size; i++) {
		C99Sphere sphere = this->spheres->at(i).get()->getC99();

		spheres[i] = sphere;
	}

	result.spheres_num = size;

	// TODO: Добавить остальное

	return result;
}

void Scene::update(sf::RenderWindow &window, sf::Time time)
{
	this->getCamera()->update(window, time);
}

std::vector<std::shared_ptr<Object>>* Scene::getSceneObjects()
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
