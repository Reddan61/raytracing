#include "Scene.h"

Scene::Scene(Camera *camera)
{
	this->SceneObjects = new std::vector<std::shared_ptr<Object>>;
	this->SceneLights = new std::vector<std::shared_ptr<Light>>;
	this->sky = new Sky();

	this->spheres = new std::vector<std::shared_ptr<Sphere>>;
	this->triangles = new std::vector<std::shared_ptr<Triangle>>;
	this->meshes = new std::vector<std::shared_ptr<TriangleMesh>>;

	this->ambientLight = nullptr;
	this->pointLights = new std::vector<std::shared_ptr<PointLight>>;
	this->directionalLights = new std::vector<std::shared_ptr<DirectionalLight>>;

	this->createGpuScene();

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
	//delete this->ambientLight;
	delete this->pointLights;
	delete this->directionalLights;
	delete this->sky;

	this->deleteGpuScene();
}

void Scene::addSphere(Sphere* sphere)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(sphere));
	this->spheres->push_back(std::shared_ptr<Sphere>(sphere));
	this->sceneChanged();
}

void Scene::addTriangle(Triangle* triangle)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(triangle));
	this->triangles->push_back(std::shared_ptr<Triangle>(triangle));
	this->sceneChanged();
}

void Scene::addMesh(TriangleMesh* mesh)
{
	this->SceneObjects->push_back(std::shared_ptr<Object>(mesh));
	this->meshes->push_back(std::shared_ptr<TriangleMesh>(mesh));
	this->sceneChanged();
}

void Scene::addAmbientLight(AmbientLight* light)
{
	this->ambientLight = std::shared_ptr<AmbientLight>(light);
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
	this->sceneChanged();
}

void Scene::addDirectionalLight(DirectionalLight* light)
{
	this->directionalLights->push_back(std::shared_ptr<DirectionalLight>(light));
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
	this->sceneChanged();
}

void Scene::addPointLight(PointLight* light)
{
	this->pointLights->push_back(std::shared_ptr<PointLight>(light));
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
	this->sceneChanged();
}

C99Scene* Scene::getGpuScene()
{
	return this->gpu_scene;
}

void Scene::createGpuScene()
{
	C99Scene* result = new C99Scene;

	const int spheres_size = this->spheres->size();

	if (spheres_size > 0) {
		C99Sphere* spheres = new C99Sphere[spheres_size];

		for (int i = 0; i < spheres_size; i++) {
			C99Sphere sphere = this->spheres->at(i).get()->getC99();

			spheres[i] = sphere;
		}

		result->spheres_num = spheres_size;
		result->spheres = spheres;
	}
	else {
		result->spheres = nullptr;
		result->spheres_num = 0;
	}


	if (this->ambientLight.get() != nullptr) {
		result->ambientLight = this->ambientLight.get()->getC99();
	}
	else {
		result->ambientLight = nullptr;
	}

	const int directional_lights_size = this->directionalLights->size();

	if (directional_lights_size > 0) {
		C99DirectionalLight* c99_directional_lights = new C99DirectionalLight[directional_lights_size];

		for (int i = 0; i < directional_lights_size; i++) {
			C99DirectionalLight light = this->directionalLights->at(i).get()->getC99();

			c99_directional_lights[i] = light;
		}

		result->directionLights = c99_directional_lights;
		result->numDirectionalLights = directional_lights_size;		
	}
	else {
		result->directionLights = nullptr;
		result->numDirectionalLights = 0;
	}


	const int point_lights_size = this->pointLights->size();

	if (point_lights_size > 0) {
		C99PointLight* c99_point_lights = new C99PointLight[point_lights_size];

		for (int i = 0; i < point_lights_size; i++) {
			C99PointLight light = this->pointLights->at(i).get()->getC99();

			c99_point_lights[i] = light;
		}

		result->pointLights = c99_point_lights;
		result->numPointLights = point_lights_size;
	}
	else {
		result->pointLights = nullptr;
		result->numPointLights = 0;
	}

	this->gpu_scene = result;
}

void Scene::deleteGpuScene()
{
	C99Scene* scene = this->gpu_scene;

	if ( scene == nullptr || scene == ((void*)0)) return;

	if (scene->spheres != nullptr) {
		delete[] scene->spheres;
	}

	if (scene->ambientLight != nullptr) {
		delete scene->ambientLight;
	}


	if (scene->directionLights != nullptr) {
		delete[] scene->directionLights;
	}

	if (scene->pointLights != nullptr) {
		delete[] scene->pointLights;
	}

	delete this->gpu_scene;
	this->gpu_scene = nullptr;
}

void Scene::sceneChanged()
{
	this->deleteGpuScene();
	this->createGpuScene();
}

void Scene::update(sf::RenderWindow &window, sf::Time time)
{
	//this->getCamera()->update(time);
}

std::vector<std::shared_ptr<Object>>* Scene::getSceneObjects()
{
	return this->SceneObjects;
}

std::vector<std::shared_ptr<Light>>* Scene::getSceneLights()
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
