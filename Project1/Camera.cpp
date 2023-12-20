#include "Camera.h"

Camera::Camera(float x, float y, float z) {
	this->position = sf::Vector3f(x, y, z);

	this->rotationX = new Matrix();
	this->rotationY = new Matrix();
	this->rotationZ = new Matrix();

	this->setRotationX(this->rotationXAngle);
	this->setRotationY(this->rotationYAngle);
	this->setRotationZ(this->rotationZAngle);
}

Camera::~Camera()
{
	delete this->rotationX;
	delete this->rotationY;
	delete this->rotationZ;
}

sf::Vector3f Camera::getPosition()
{
	return this->position;
}

void Camera::update(sf::RenderWindow& window, sf::Time time)
{
	this->keyCheck(time);
}

Matrix Camera::getRotation()
{
	return *this->rotationX * *this->rotationY * *this->rotationZ;
}

void Camera::keyCheck(sf::Time time)
{
	auto seconds = time.asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->position.z += seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->position.z -= seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->position.x -= seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->position.x += seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->position.y += seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		this->position.y -= seconds * this->SPEEDMOVE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->setRotationY(this->rotationYAngle - seconds * this->SPEEDSENS);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->setRotationY(this->rotationYAngle + seconds * this->SPEEDSENS);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->setRotationX(this->rotationXAngle - seconds * this->SPEEDSENS);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->setRotationX(this->rotationXAngle + seconds * this->SPEEDSENS);
	}
}

void Camera::setRotationX(float angle)
{
	this->rotationXAngle = angle;

	this->rotationX->setItem(0, 0, 1);
	this->rotationX->setItem(1, 1, std::cos(this->rotationXAngle));
	this->rotationX->setItem(1, 2, -std::sin(this->rotationXAngle));
	this->rotationX->setItem(2, 1, std::sin(this->rotationXAngle));
	this->rotationX->setItem(2, 2, std::cos(this->rotationXAngle));
}

void Camera::setRotationY(float angle)
{
	this->rotationYAngle = angle;

	this->rotationY->setItem(0, 0, std::cos(this->rotationYAngle));
	this->rotationY->setItem(0, 2, std::sin(this->rotationYAngle));
	this->rotationY->setItem(1, 1, 1);
	this->rotationY->setItem(2, 0, -std::sin(this->rotationYAngle));
	this->rotationY->setItem(2, 2, std::cos(this->rotationYAngle));
}

void Camera::setRotationZ(float angle)
{
	this->rotationZAngle = angle;

	this->rotationZ->setItem(0, 0, std::cos(this->rotationZAngle));
	this->rotationZ->setItem(0, 1, -std::sin(this->rotationZAngle));
	this->rotationZ->setItem(1, 0, std::sin(this->rotationZAngle));
	this->rotationZ->setItem(1, 1, std::cos(this->rotationZAngle));
	this->rotationZ->setItem(2, 2, 1);
}

//void Camera::render(sf::RenderWindow& window)
//{
//	window.draw(this->object);
//}

//void Camera::changePosition(float x, float y)
//{
//	this->object.setPosition(position);
//}
//
//void Camera::changeAngle(float angle)
//{
//	this->angle = angle;
//}
