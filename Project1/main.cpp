#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Scene.h"
#include "Object.h"
#include "Camera.h"
#include "Render.h"
#include "Sphere.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");

	auto* objects = new std::vector<Object*>();

	objects->push_back(new Sphere(sf::Vector3f(0, -1, 3), sf::Color(255,0 ,0, 255)));
	objects->push_back(new Sphere(sf::Vector3f(2, 0, 4), sf::Color(0, 0, 255, 255)));
	objects->push_back(new Sphere(sf::Vector3f(-2, 0, 4), sf::Color(0, 255, 0, 255)));

	Scene* scene = new Scene(objects, new Camera(0, 0, 0));

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
	}

	return 0;
}