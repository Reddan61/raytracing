#pragma once
#include <vector>
#include "./Objects/Object.h"
#include "Camera.h"
#include "Light/Light.h"
#include "Light/AmbientLight.h"
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
	std::vector<Light*>* getSceneLights();
	Camera* getCamera();
	Sky* getSky();

	void addSphere(Sphere* sphere);
	void addTriangle(Triangle* triangle);
	void addMesh(TriangleMesh* mesh);

	void addLight(Light* light);

	C99Scene getGpuScene();

	void update(sf::RenderWindow &window, sf::Time time);
private:
	std::vector<std::shared_ptr<Object>> *SceneObjects = nullptr;
	std::vector<Light*> *SceneLights = nullptr;

	std::vector<std::shared_ptr<Sphere>> *spheres = nullptr;
	std::vector<std::shared_ptr<Triangle>> *triangles = nullptr;
	std::vector<std::shared_ptr<TriangleMesh>> *meshes = nullptr;

	Camera* camera = nullptr;
	Sky* sky = nullptr;

	//void renderObjects(sf::RenderWindow &window, sf::Time time);
};

