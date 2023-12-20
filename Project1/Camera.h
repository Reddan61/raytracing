#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "./Matrix/Matrix.h"

class Camera
{
public:
	Camera(float x, float y, float z);
	~Camera();

	sf::Vector3f getPosition();
	void update(sf::RenderWindow& window, sf::Time time);
	Matrix getRotation();

private:
	sf::Vector3f position;
	//sf::CircleShape object;
	const float SPEEDMOVE = 0.2;
	const float SPEEDSENS = 0.2;
	float rotationXAngle = 0;
	float rotationYAngle = 0;
	float rotationZAngle = 0;


	Matrix *rotationY = nullptr;
	Matrix *rotationX = nullptr;
	Matrix *rotationZ = nullptr;

	void keyCheck(sf::Time time);

	void setRotationX(float angle);
	void setRotationY(float angle);
	void setRotationZ(float angle);
	//void changePosition(float x, float y);
	//void changeAngle(float angle);
};

