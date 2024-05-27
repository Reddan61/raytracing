#include <iostream>
#include <vector>
#include "Scene.h"
#include "Camera.h"
#include "./Objects/Sphere/Sphere.h"
#include "./Objects/Object.h"
#include "./Objects/Triangle/Triangle.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "./Window/Window.h"
#include "./Utils.h"

int main() {
    const uint32_t WIDTH = 1200;
    const uint32_t HEIGHT = 800;

    Camera *camera = new Camera(0.0f, 2.0f, 5.0f);

    AmbientLight* ambient_light = new AmbientLight(0.1f);
    DirectionalLight* directional_light = new DirectionalLight(glm::vec3(0.0f, 1.0f, 1.0f), 0.2f, 2.0f);

    Scene* scene = new Scene(camera, ambient_light, directional_light);
    scene->setAA(1);

    Sphere* sphere1 = new Sphere(glm::vec3(0.0, 1.0, -6.0), glm::vec3(1.0, 0.0, 0.0), 1.0f, 0.2f, 0.5f);
    Sphere* sphere2 = new Sphere(glm::vec3(2.5, 0.0, -6.0), glm::vec3(0.0, 1.0, 0.0), 1.0f, -1.0f, 0.5f);
    Sphere* sphere3 = new Sphere(glm::vec3(-2.5, 0.0, -6.0), glm::vec3(0.0, 0.0, 1.0), 1.0f, -1.0f, 0.0f);
    Sphere* sphere4 = new Sphere(glm::vec3(0, -5002.0f, 0.0), glm::vec3(1.0, 1.0, 0.0), 5000.0, 0.2f, 0.0f);
    Sphere* sphere5 = new Sphere(glm::vec3(0.0, 3.0f, -6.0), glm::vec3(0.0, 0.0, 1.0), 1.0f, -1.0f, 0.0f);
    Sphere* sphere6 = new Sphere(glm::vec3(2.5f, 3.0f, -6.0), glm::vec3(1.0, 0.0, 0.0), 1.0f, -1.0f, 0.0f);
    Sphere* sphere7 = new Sphere(glm::vec3(-2.5f, 3.0f, -6.0), glm::vec3(0.0, 1.0, 0.0), 1.0f, -1.0f, 0.0f);

    TriangleMesh* sus = new TriangleMesh(Utils::loadOBJ("sus2.obj"));
    TriangleMesh* sus2 = new TriangleMesh(Utils::loadOBJ("sus2.obj"));
    TriangleMesh* sus3 = new TriangleMesh(Utils::loadOBJ("sus2.obj"));
    TriangleMesh* sus4 = new TriangleMesh(Utils::loadOBJ("sus2.obj"));
    sus->changePosition(glm::vec3(5.0f, 0.0f, -5.0f));
    sus->rotate(0.0f, 180.0f);
    sus2->changePosition(glm::vec3(-5.0f, 0.0f, -5.0f));
    sus3->changePosition(glm::vec3(0.0f, 0.0f, 2.0f));
    sus4->changePosition(glm::vec3(0.0f, 0.0f, -10.f));
    sus3->rotate(0.0f, -90.0f);
    sus2->rotate(0.0f, -180.0f);

    PointLight* pointLight1 = new PointLight(glm::vec3(4.0f, 10.0f, 0.0), 0.4f, 128.0f);
    PointLight* pointLight2 = new PointLight(glm::vec3(-4.0f, 2.0f, 0.0), 0.4f, 32.0f);
    PointLight* pointLight3 = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), 0.4f, 32.0f);
    PointLight* pointLight4 = new PointLight(glm::vec3(0.0f, 4.0f, 0.0f), 0.4f, 128.0f);
    PointLight* pointLight5 = new PointLight(glm::vec3(0.0f, 2.0f, -3.0f), 0.4f, 128.0f);
    PointLight* pointLight6 = new PointLight(glm::vec3(0.0f, 2.0f, 3.0f), 0.4f, 128.0f);

    Animation* sphere1_animation = new Animation(sphere1->getPosition(), glm::vec3(10.0f, 1.0f, -6.0f), 1.0f, true);
    sphere1->setAnimation(sphere1_animation);
    
    Animation* sphere2_animation = new Animation(sphere2->getPosition(), glm::vec3(2.5, 10.0, -6.0), 1.0f, true);
    sphere2->setAnimation(sphere2_animation);

    scene->addSphere(sphere1);
    scene->addSphere(sphere2);
    //scene->addSphere(sphere2);
    //scene->addSphere(sphere3);
    scene->addSphere(sphere4);
    //scene->addSphere(sphere5);
    /*scene->addSphere(sphere6);
    scene->addSphere(sphere7);*/
    scene->addMesh(sus);
    scene->addMesh(sus2);
    scene->addMesh(sus3);
    scene->addMesh(sus4);
    //scene->addMesh(test);

    scene->addPointLight(pointLight1);
    //scene->addPointLight(pointLight2);
    //scene->addPointLight(pointLight3);
    //scene->addPointLight(pointLight4);
    //scene->addPointLight(pointLight5);
    //scene->addPointLight(pointLight6);

    Window window = Window(WIDTH, HEIGHT, scene);

    window.run();

    return 0;
}

//int main()
//{
//	int width = 1200;
//	int height = 800;
//
//	sf::RenderWindow window(sf::VideoMode(width, height), "SFML works!");
//
//	ThreadPool pool(12);
//	GPU gpu;
//
//	auto* objects = new std::vector<Object*>();
//	auto* lights = new std::vector<Light*>();
//
//	TriangleMesh* cube = Utils::LoadCustomFormatFile("cube.txt");
//	cube->changePosition(Vector3d(2, 1, 0));
//
//	Scene* scene = new Scene(new Camera(0, 0, 0));
//
//	scene->addSphere(new Sphere(Vector3d(-1, 1, 0), Color(255, 255, 0, 255)));
//	scene->addSphere(new Sphere(Vector3d(2, 4, 0), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(Vector3d(2, 0, -3), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(Vector3d(2, 1, -3), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(Vector3d(-2, 1, -3), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(Vector3d(2, 1, 3), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(Vector3d(-2, 1, 3), Color(0, 255, 0, 255)));
//	//scene->addSphere(new Sphere(
//	//	Vector3d(0, -5001, 0),
//	//	Color(255, 255, 0, 255),
//	//	5000,
//	//	1000,
//	//	0.5
//	//));
//
//	//scene->addTriangle(new Triangle(Vector3d(0, 0, -3), Vector3d(1, 1, -3), Vector3d(2, 0, -3), Color(0, 0, 255, 255)));
//	//scene->addTriangle(new Triangle(Vector3d(1, 0, 1), Vector3d(2, 1, 1), Vector3d(3, 0, 1), Color(0, 0, 255, 255)));
//	//scene->addTriangle(new Triangle(Vector3d(1, 0, -1), Vector3d(2, 1, -1), Vector3d(3, 0, -1), Color(0, 0, 255, 255)));
//
//	//scene->addMesh(cube);
//
//	scene->addAmbientLight(new AmbientLight(0.2f));
//	scene->addPointLight(new PointLight(Vector3d(3, 4, -3), 0.8f));
//
//	//lights->push_back(new PointLight(Vector3d(2, 1, 0), 0.6f));
//	//lights->push_back(new DirectionalLight(Vector3d(1, 4, 4), 0.2f));
//
//	sf::Clock clock;
//	Render *render = new Render(width, height);
//
//	int i = 0;
//
//	//while (window.isOpen())
//	while (i == 0)
//	{
//		i++;
//		sf::Event event;
//		sf::Time time = clock.restart();
//
//		while (window.pollEvent(event)) {
//			if (event.type == sf::Event::Closed)
//				window.close();
//		}
//
//		window.clear();
//
//		render->update(scene, window, time, pool, gpu);
//
//		window.display();
//		
//		std::cout << "fps: " << 1.f / clock.getElapsedTime().asSeconds() << std::endl;
//	}
//
//	return 0;
//}