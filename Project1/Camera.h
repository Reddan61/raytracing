#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Math.h"
#include "./Matrix/Matrix3d.h"

class Camera
{
public:
	Camera(float x, float y, float z);
	~Camera();

	sf::Vector3f getPosition();
	void update(sf::RenderWindow& window, sf::Time time);
	Matrix4d getRotation();

private:
	sf::Vector3f position;
	sf::Vector3f front;
	sf::Vector3f up;

	//sf::CircleShape object;
	const float SPEEDMOVE = 0.2;
	const float SPEEDSENS = 5;
	float rotationXAngle = 0;
	float rotationYAngle = 0;

	Matrix4d *rotationY = nullptr;
	Matrix4d *rotationX = nullptr;

	void keyCheck(sf::Time time);

	void setRotationX();
	void setRotationY();

	void calculatedRotations();
	void calculateFront();
	void calculateUp();
};

