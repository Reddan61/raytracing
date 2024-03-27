#include "Scene.h"

Scene::Scene(Camera *camera)
{
	//this->SceneObjects = new std::vector<std::shared_ptr<Object>>;
	//this->SceneLights = new std::vector<std::shared_ptr<Light>>;
	//this->sky = new Sky();

	this->spheres = new std::vector<std::shared_ptr<Sphere>>;
	//this->triangles = new std::vector<std::shared_ptr<Triangle>>;
	//this->meshes = new std::vector<std::shared_ptr<TriangleMesh>>;

	//this->ambientLight = nullptr;
	//this->pointLights = new std::vector<std::shared_ptr<PointLight>>;
	//this->directionalLights = new std::vector<std::shared_ptr<DirectionalLight>>;

	this->camera = camera;
}

Scene::~Scene()
{
	delete this->camera;
	//delete this->SceneObjects;
	//delete this->SceneLights;
	delete this->spheres;
	/*delete this->triangles;
	delete this->meshes;*/
	//delete this->ambientLight;
	//delete this->pointLights;
	//delete this->directionalLights;
	//delete this->sky;
}

void Scene::addSphere(Sphere* sphere)
{
	//this->SceneObjects->push_back(std::shared_ptr<Object>(sphere));
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

std::vector<Sphere::SphereShader> Scene::getSpheresBuffer()
{
	auto result = std::vector<Sphere::SphereShader>();

	for (int i = 0; i < this->spheres->size(); i++) {
		result.push_back(this->spheres->at(i).get()->getSphereShader());
	}

	return result;
}

VkDeviceSize Scene::getSphereBufferSize()
{
	VkDeviceSize size = sizeof(Sphere::SphereShader) * this->spheres->size();

	return size;
}

Scene::SceneShader Scene::getSceneBuffer()
{
	Scene::SceneShader result;

	result.spheres_num = this->spheres->size();
	result.camera = this->camera->getBufferStruct();

	return result;
}

VkDeviceSize Scene::getSceneBufferSize()
{
	VkDeviceSize size = sizeof(Scene::SceneShader);

	return size;
}

void Scene::addAmbientLight(AmbientLight* light)
{
	this->ambientLight = std::shared_ptr<AmbientLight>(light);
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
}

void Scene::addDirectionalLight(DirectionalLight* light)
{
	this->directionalLights->push_back(std::shared_ptr<DirectionalLight>(light));
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
}

void Scene::addPointLight(PointLight* light)
{
	this->pointLights->push_back(std::shared_ptr<PointLight>(light));
	this->SceneLights->push_back(std::shared_ptr<Light>(light));
}


void Scene::update(GLFWwindow* window, float delta)
{
	this->camera->update(window, delta);
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
