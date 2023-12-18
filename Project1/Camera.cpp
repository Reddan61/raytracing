#include "Camera.h"

Camera::Camera(float x, float y, float z) {
	this->position = sf::Vector3f(x, y, z);
}

sf::Vector3f Camera::getPosition()
{
	return this->position;
}

//void Camera::update(sf::RenderWindow& window, sf::Time time)
//{
//	this->keyCheck(time);
//	this->render(window);
//}

//void Camera::keyCheck(sf::Time time)
//{
//	auto position = this->object.getPosition();
//	auto seconds = time.asSeconds();
//	float newAngle = this->angle;
//
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
//		position.y -= seconds * this->SPEEDMOVE;
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
//		position.y += seconds * this->SPEEDMOVE;
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//		position.x -= seconds * this->SPEEDMOVE;
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//		position.x += seconds * this->SPEEDMOVE;
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
//		newAngle -= seconds * this->SPEEDSENS;
//	}
//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
//		newAngle += seconds * this->SPEEDSENS;
//	}
//
//	if (newAngle != angle) {
//		this->changeAngle(newAngle);
//	}
//
//	//this->changePosition(position);
//}

//void Camera::render(sf::RenderWindow& window)
//{
//	window.draw(this->object);
//}

//void Camera::changePosition(sf::Uint16 x, sf::Uint16 y)
//{
//	//this->object.setPosition(position);
//}
//
//void Camera::changeAngle(float angle)
//{
//	this->angle = angle;
//}
