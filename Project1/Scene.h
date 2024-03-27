#pragma once
#include <vector>
#include "./Objects/Object.h"
#include "Camera.h"
#include "Light/Light.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Sky/Sky.h"
#include "Objects/Sphere/Sphere.h"
#include "Objects/Triangle/Triangle.h"
#include "Objects/TriangleMesh/TriangleMesh.h"

class Scene
{
public:
	struct SceneShader {
		int spheres_num;
		Camera::CameraVulkan camera;
	};

	Scene(Camera *camera);
	~Scene();

	std::vector<std::shared_ptr<Object>>* getSceneObjects();
	std::vector<std::shared_ptr<Light>>* getSceneLights();
	Camera* getCamera();
	Sky* getSky();

	void addSphere(Sphere* sphere);
	void addTriangle(Triangle* triangle);
	void addMesh(TriangleMesh* mesh);

	std::vector<Sphere::SphereShader> getSpheresBuffer();
	VkDeviceSize getSphereBufferSize();

	Scene::SceneShader getSceneBuffer();
	VkDeviceSize getSceneBufferSize();

	void addAmbientLight(AmbientLight* light);
	void addDirectionalLight(DirectionalLight* light);
	void addPointLight(PointLight* light);

	void update(GLFWwindow* window, float delta);
private:
	std::vector<std::shared_ptr<Object>> *SceneObjects = nullptr;
	std::vector<std::shared_ptr<Light>> *SceneLights = nullptr;

	std::vector<std::shared_ptr<Sphere>> *spheres = nullptr;
	std::vector<std::shared_ptr<Triangle>> *triangles = nullptr;
	std::vector<std::shared_ptr<TriangleMesh>> *meshes = nullptr;

	std::shared_ptr<AmbientLight> ambientLight = nullptr;
	std::vector<std::shared_ptr<PointLight>>* pointLights = nullptr;
	std::vector<std::shared_ptr<DirectionalLight>>* directionalLights = nullptr;
	
	Camera* camera = nullptr;
	Sky* sky = nullptr;
};

