#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Render.h"
#include "Sphere.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

	auto* objects = new std::vector<Object*>();
	auto* lights = new std::vector<Light*>();

	objects->push_back(new Sphere(sf::Vector3f(0, -1, 3), sf::Color(255,0 ,0, 255), 1, 500));
	objects->push_back(new Sphere(sf::Vector3f(2, 0, 4), sf::Color(0, 0, 255, 255), 1, 500));
	objects->push_back(new Sphere(sf::Vector3f(-2, 0, 4), sf::Color(0, 255, 0, 255), 1, 10));
	objects->push_back(new Sphere(sf::Vector3f(0, -5001, 0), sf::Color(255, 255, 0, 255), 5000, 100));

	lights->push_back(new AmbientLight(0.2f));
	lights->push_back(new PointLight(sf::Vector3f(2, 1, 0), 0.6f));
	lights->push_back(new DirectionalLight(sf::Vector3f(1, 4, 4), 0.2f));

	Scene* scene = new Scene(objects, lights, new Camera(0, 0, 0));

	sf::Clock clock;
	Render *render = new Render(600, 600);

	while (window.isOpen())
	{
		sf::Event event;
		sf::Time time = clock.restart();

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();

		render->update(scene, window, time);

		window.display();
		
		std::cout << "fps: " << 1.f / clock.getElapsedTime().asSeconds() << std::endl;
	}

	return 0;
}