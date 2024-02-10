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

extern "C" {
	#include "C99/structs.h"
}

class Scene
{
public:
	Scene(Camera *camera);
	~Scene();

	std::vector<std::shared_ptr<Object>>* getSceneObjects();
	std::vector<std::shared_ptr<Light>>* getSceneLights();
	Camera* getCamera();
	Sky* getSky();

	void addSphere(Sphere* sphere);
	void addTriangle(Triangle* triangle);
	void addMesh(TriangleMesh* mesh);

	void addAmbientLight(AmbientLight* light);
	void addDirectionalLight(DirectionalLight* light);
	void addPointLight(PointLight* light);

	C99Scene* getGpuScene();

	void update(sf::RenderWindow &window, sf::Time time);
private:
	std::vector<std::shared_ptr<Object>> *SceneObjects = nullptr;
	std::vector<std::shared_ptr<Light>> *SceneLights = nullptr;

	std::vector<std::shared_ptr<Sphere>> *spheres = nullptr;
	std::vector<std::shared_ptr<Triangle>> *triangles = nullptr;
	std::vector<std::shared_ptr<TriangleMesh>> *meshes = nullptr;

	std::shared_ptr<AmbientLight> ambientLight = nullptr;
	std::vector<std::shared_ptr<PointLight>>* pointLights = nullptr;
	std::vector<std::shared_ptr<DirectionalLight>>* directionalLights = nullptr;

	C99Scene* gpu_scene = nullptr;
	
	Camera* camera = nullptr;
	Sky* sky = nullptr;

	void sceneChanged();
	void createGpuScene();
	void deleteGpuScene();

	//void renderObjects(sf::RenderWindow &window, sf::Time time);
};

