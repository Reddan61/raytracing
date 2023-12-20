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
#include "Matrix/Matrix.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

	auto* objects = new std::vector<Object*>();
	auto* lights = new std::vector<Light*>();

	objects->push_back(new Sphere(sf::Vector3f(0, -1, 3), sf::Color(255, 0 ,0, 255), 1, 500, 0.2));
	objects->push_back(new Sphere(sf::Vector3f(2, 0, 4), sf::Color(0, 0, 255, 255), 1, 500, 0.3));
	objects->push_back(new Sphere(sf::Vector3f(-2, 0, 4), sf::Color(0, 255, 0, 255), 1, 10, 0.4));
	objects->push_back(new Sphere(sf::Vector3f(0, -5001, 0), sf::Color(255, 255, 0, 255), 5000, 1000, 0.5));

	lights->push_back(new AmbientLight(0.2f));
	lights->push_back(new PointLight(sf::Vector3f(2, 1, 0), 0.6f));
	lights->push_back(new DirectionalLight(sf::Vector3f(1, 4, 4), 0.2f));

	Scene* scene = new Scene(objects, lights, new Camera(0, 0, 0));

	sf::Clock clock;
	Render *render = new Render(600, 600);

	//Matrix m1;
	////Matrix m2;

	//m1.setItem(0, 0, 1);
	//m1.setItem(0, 1, 2);
	//m1.setItem(0, 2, 3);
	//m1.setItem(1, 0, 4);
	//m1.setItem(1, 1, 5);
	//m1.setItem(1, 2, 6);
	//m1.setItem(2, 0, 7);
	//m1.setItem(2, 1, 8);
	//m1.setItem(2, 2, 9);

	////m2.setItem(0, 0, 10);
	////m2.setItem(0, 1, 20);
	////m2.setItem(0, 2, 30);
	////m2.setItem(1, 0, 40);
	////m2.setItem(1, 1, 50);
	////m2.setItem(1, 2, 60);
	////m2.setItem(2, 0, 70);
	////m2.setItem(2, 1, 80);
	////m2.setItem(2, 2, 90);

	////auto result = m1 * m2;

	//auto result = m1 * sf::Vector3f(10, 20, 30);

	//////std::cout << m2.getItem(2, 1);
	////result.showItems();

	//int test;

	//std::cin >> test;

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