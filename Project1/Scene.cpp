#include "Scene.h"

Scene::Scene(Camera *camera)
{
	//this->SceneObjects = new std::vector<std::shared_ptr<Object>>;
	//this->SceneLights = new std::vector<std::shared_ptr<Light>>;
	//this->sky = new Sky();

	this->spheres = new std::vector<std::shared_ptr<Sphere>>;
	this->triangles = new std::vector<std::shared_ptr<Triangle>>;
	this->meshes = new std::vector<std::shared_ptr<TriangleMesh>>;

	//this->ambientLight = nullptr;
	this->pointLights = new std::vector<std::shared_ptr<PointLight>>;
	//this->directionalLights = new std::vector<std::shared_ptr<DirectionalLight>>;

	this->camera = camera;
}

Scene::~Scene()
{
	delete this->camera;
	delete this->spheres;
	delete this->triangles;
	delete this->pointLights;
	delete this->meshes;
	//delete this->SceneObjects;
	//delete this->SceneLights;
	//delete this->ambientLight;
	//delete this->directionalLights;
	//delete this->sky;
}

void Scene::addSphere(Sphere* sphere)
{
	this->spheres->push_back(std::shared_ptr<Sphere>(sphere));
}

void Scene::addTriangle(Triangle* triangle)
{
	this->triangles->push_back(std::shared_ptr<Triangle>(triangle));
}

void Scene::addMesh(TriangleMesh* mesh)
{
	this->meshes->push_back(std::shared_ptr<TriangleMesh>(mesh));
}

std::vector<Sphere::SphereShader> Scene::getSpheresBuffer()
{
	auto result = std::vector<Sphere::SphereShader>();

	for (int i = 0; i < this->spheres->size(); i++) {
		result.push_back(this->spheres->at(i).get()->getShader());
	}

	return result;
}

VkDeviceSize Scene::getSpheresBufferSize()
{
	VkDeviceSize size = Sphere::getBufferSize() * this->spheres->size();

	return size;
}

std::vector<Triangle::TriangleShader> Scene::getTrianglesBuffer()
{
	auto result = std::vector<Triangle::TriangleShader>();

	result.reserve(this->getTrianglesNum());

	for (int i = 0; i < this->triangles->size(); i++) {
		result.push_back(this->triangles->at(i).get()->getShader());
	}

	for (int i = 0; i < this->meshes->size(); i++) {
		auto polygons = this->meshes->at(i).get()->getPolygons();

		for (int j = 0; j < polygons->size(); j++) {
			result.push_back(polygons->at(j)->getShader());
		}
	} 

	return result;
}

VkDeviceSize Scene::getTrianglesBufferSize()
{
	VkDeviceSize size = Triangle::getBufferSize() * this->getTrianglesNum();

	return size;
}

std::vector<PointLight::PointLightShader> Scene::getPointLightsBuffer()
{
	auto result = std::vector<PointLight::PointLightShader>();

	for (int i = 0; i < this->pointLights->size(); i++) {
		result.push_back(this->pointLights->at(i).get()->getBuffer());
	}

	return result;
}

VkDeviceSize Scene::getPointLightsBufferSize()
{
	VkDeviceSize size = PointLight::getBufferSize() * this->pointLights->size();

	return size;
}

Scene::SceneShader Scene::getSceneBuffer()
{
	Scene::SceneShader result;

	result.aa = this->aa;
	result.spheres_num = this->spheres->size();
	result.triangles_num = this->getTrianglesNum();
	result.point_ligts_num = this->pointLights->size();
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
	//this->SceneLights->push_back(std::shared_ptr<Light>(light));
}

void Scene::setAA(int num)
{
	this->aa = num;
}


void Scene::update(GLFWwindow* window, float delta)
{
	this->camera->update(window, delta);
}

size_t Scene::getTrianglesNum()
{
	size_t result = this->triangles->size();

	for (int i = 0; i < this->meshes->size(); i++) {
		result += this->meshes->at(i).get()->getPolygonsSize();
	}

	return result;
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
