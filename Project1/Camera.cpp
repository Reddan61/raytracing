#include "Camera.h"

Camera::Camera(float x, float y, float z) {
	this->position = sf::Vector3f(x, y, z);

	this->rotationX = new Matrix4d();
	this->rotationY = new Matrix4d();

	this->front = sf::Vector3f(0, 0, -1.f);
	this->up = sf::Vector3f(0, -1.f, 0);

	this->setRotationX(0);
	this->setRotationY(0);
	this->calculatedRotations();
}

Camera::~Camera()
{
	delete this->rotationX;
	delete this->rotationY;
}

sf::Vector3f Camera::getPosition()
{
	return this->position;
}

void Camera::update(sf::RenderWindow& window, sf::Time time)
{
	this->keyCheck(time);
}

Matrix4d Camera::getRotation()
{
	return *this->rotationY * *this->rotationX;
}

void Camera::keyCheck(sf::Time time)
{
	auto seconds = time.asSeconds();
	bool moved = false;
	bool rotated = false;
	sf::Vector3f right = Math::normalize(Math::GetCrossProduct(this->front, this->up));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->position -= (this->SPEEDMOVE * seconds) * this->front;
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->position += (this->SPEEDMOVE * seconds) * this->front;
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->position += right * (this->SPEEDMOVE * seconds);
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->position -= right * (this->SPEEDMOVE * seconds);
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->position -= this->up * (this->SPEEDMOVE * seconds);
		moved = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
		this->position += this->up * (this->SPEEDMOVE * seconds);
		moved = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		this->setRotationX(this->rotationXAngle + this->SPEEDSENS);
		rotated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		this->setRotationX(this->rotationXAngle - this->SPEEDSENS);
		rotated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		this->setRotationY(this->rotationYAngle + this->SPEEDSENS);
		rotated = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		this->setRotationY(this->rotationYAngle - this->SPEEDSENS);
		rotated = true;
	}	

	if (rotated) {
		this->calculatedRotations();
	}

}

void Camera::setRotationX(float angle)
{
	this->rotationXAngle = angle;
	float radians = Math::GetRadians(this->rotationXAngle);

	this->rotationX->setItem(0, 0, std::cosf(radians));
	this->rotationX->setItem(0, 2, std::sinf(radians));
	this->rotationX->setItem(1, 1, 1);
	this->rotationX->setItem(2, 0, -std::sinf(radians));
	this->rotationX->setItem(2, 2, std::cosf(radians));
	this->rotationX->setItem(3, 3, 1);
}

void Camera::setRotationY(float angle)
{
	int result = angle;

	if (result > 89) {
		result = 89;
	}
	else if (result < -89) {
		result = -89;
	}

	this->rotationYAngle = result;

	float radians = Math::GetRadians(this->rotationYAngle);

	this->rotationY->setItem(0, 0, 1);
	this->rotationY->setItem(1, 1, std::cosf(radians));
	this->rotationY->setItem(1, 2, -std::sinf(radians));
	this->rotationY->setItem(2, 1, std::sinf(radians));
	this->rotationY->setItem(2, 2, std::cosf(radians));
	this->rotationY->setItem(3, 3, 1);
}

void Camera::calculatedRotations()
{
	this->calculateFront();
	this->calculateUp();
}

void Camera::calculateFront()
{
	this->front = Math::normalize(this->getRotation() * sf::Vector3f(0, 0, -1.f));
}

void Camera::calculateUp()
{
	this->up = Math::normalize(this->getRotation() * sf::Vector3f(0, -1.f, 0));
}
