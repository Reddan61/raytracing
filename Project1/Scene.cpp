#include "Scene.h"

Scene::Scene(Camera *camera, AmbientLight* ambient_light, DirectionalLight* directional_light)
{
	//this->SceneObjects = new std::vector<std::shared_ptr<Object>>;
	//this->SceneLights = new std::vector<std::shared_ptr<Light>>;
	//this->sky = new Sky();

	this->spheres = new std::vector<std::shared_ptr<Sphere>>;
	//this->triangles = new std::vector<std::shared_ptr<Triangle>>;
	this->meshes = new std::vector<std::shared_ptr<TriangleMesh>>;

	//this->ambientLight = nullptr;
	this->pointLights = new std::vector<std::shared_ptr<PointLight>>;
	//this->directionalLights = new std::vector<std::shared_ptr<DirectionalLight>>;

	this->camera = camera;
	this->setAmbientLight(ambient_light);
	this->setDirectionalLight(directional_light);
}

Scene::~Scene()
{
	delete this->camera;
	delete this->spheres;
	//delete this->triangles;
	delete this->pointLights;
	delete this->meshes;

	delete this->bvhs_shader;
	//delete this->SceneObjects;
	//delete this->SceneLights;
	//delete this->sky;
}

void Scene::addSphere(Sphere* sphere)
{
	this->spheres->push_back(std::shared_ptr<Sphere>(sphere));
}

//void Scene::addTriangle(Triangle* triangle)
//{
//	this->triangles->push_back(std::shared_ptr<Triangle>(triangle));
//}

void Scene::addMesh(TriangleMesh* mesh)
{
	this->meshes->push_back(std::shared_ptr<TriangleMesh>(mesh));

	this->update_triangles_num(mesh->getTrianglesNum());
	this->updateSceneBVHsBuffer();
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

	for (int i = 0; i < this->meshes->size(); i++) {
		auto polygons = this->meshes->at(i).get()->getTriangles();

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

	auto bvhs = this->getSceneBVHsBuffer();

	result.aa = this->aa; 
	result.shadow_rays = this->shadow_rays;
	result.spheres_num = this->spheres->size();
	result.triangles_num = this->getTrianglesNum();
	result.point_ligts_num = this->pointLights->size();
	result.camera = this->camera->getBufferStruct();
	result.bvh_origins = bvhs->origins.size();
	result.bvh_leaves = bvhs->leaves.size();
	result.ambient_bright = this->ambientLight.get()->getBright();
	result.directional_light = this->directionalLight.get()->getBuffer();

	return result;
}

VkDeviceSize Scene::getSceneBufferSize()
{
	VkDeviceSize size = sizeof(Scene::SceneShader);

	return size;
}

Scene::BVHsShader* Scene::getSceneBVHsBuffer()
{
	return this->bvhs_shader;
}

void Scene::updateSceneBVHsBuffer()
{
	if (this->bvhs_shader == nullptr) {
		this->bvhs_shader = new Scene::BVHsShader();
	}

	size_t triangles_offset = 0;
	size_t bvh_offset = 0;

	this->bvhs_shader->origin_size = 0;
	this->bvhs_shader->leaves_size = 0;
	this->bvhs_shader->origins.clear();
	this->bvhs_shader->origins.shrink_to_fit();	
	this->bvhs_shader->leaves.clear();
	this->bvhs_shader->leaves.shrink_to_fit();

	for (size_t i = 0; i < this->meshes->size(); i++) {
		TriangleMesh* mesh = this->meshes->at(i).get();

		TriangleMesh::BVHBuffer buffer = mesh->getBVHBuffer(bvh_offset, triangles_offset);

		bvh_offset += buffer.bvhs.size();
		triangles_offset += mesh->getTrianglesNum();

		this->bvhs_shader->origins.push_back(buffer.origin);
		this->bvhs_shader->leaves.insert(this->bvhs_shader->leaves.end(), buffer.bvhs.begin(), buffer.bvhs.end());
	}

	this->bvhs_shader->origin_size = this->bvhs_shader->origins.size() * sizeof(TriangleMesh::BVHShader);
	this->bvhs_shader->leaves_size = this->bvhs_shader->leaves.size() * sizeof(TriangleMesh::BVHShader);
}

void Scene::setAmbientLight(AmbientLight* light)
{
	this->ambientLight = std::shared_ptr<AmbientLight>(light);
}

void Scene::setDirectionalLight(DirectionalLight* light)
{
	this->directionalLight = std::shared_ptr<DirectionalLight>(light);
}

void Scene::addPointLight(PointLight* light)
{
	this->pointLights->push_back(std::shared_ptr<PointLight>(light));
}

void Scene::setAA(int num)
{
	if (num <= 0) {
		num = 1;
	}

	this->aa = num;
}

void Scene::setSoftShadowRays(int num)
{
	if (num <= 0) {
		num = 1;
	}
	else if (num > 8) {
		num = 8;
	}

	this->shadow_rays = num;
}

std::vector<std::shared_ptr<Sphere>>* Scene::getSpheres()
{
	return this->spheres;
}


void Scene::update(GLFWwindow* window, float delta)
{
	this->camera->update(window, delta);
	
	if (this->spheres != nullptr) {
		for (size_t i = 0; i < this->spheres->size(); i++) {
			this->spheres->at(i).get()->update(delta);
		}
	}
}

size_t Scene::getTrianglesNum()
{
	return this->triangles_num;
}

void Scene::update_triangles_num(size_t plus_num)
{
	this->triangles_num += plus_num;
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
