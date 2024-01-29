#pragma once
#include <SFML/Graphics.hpp>
#include "./Objects/Object.h"
#include "Math.h"
#include "./Utils/Matrix/Matrix3d.h"
#include "./Utils/Vector/Vector3d.h"

class Camera
{
public:
	Camera(float x, float y, float z);
	~Camera();

	Vector3d getPosition();
	void update(sf::RenderWindow& window, sf::Time time);
	Matrix4d getRotation();

private:
	Vector3d position;
	Vector3d front;
	Vector3d up;

	//sf::CircleShape object;
	const float SPEEDMOVE = 0.2;
	const float SPEEDSENS = 10;
	float rotationXAngle = 0;
	float rotationYAngle = 0;

	Matrix4d *rotationY = nullptr;
	Matrix4d *rotationX = nullptr;

	void keyCheck(sf::Time time);

	void setRotationX(float angle);
	void setRotationY(float angle);

	void calculatedRotations();
	void calculateFront();
	void calculateUp();
};

