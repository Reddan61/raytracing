#pragma once
#include <iostream>
#include "../Vector/Vector4d.h"

class Matrix4d
{
public:
	Matrix4d(float initValue = 0);
	~Matrix4d();

	void setItem(int y, int x, float value);
	float getItem(int y, int x) const;
	void showItems();

	Matrix4d operator *(const Matrix4d& other);
	sf::Vector3f operator *(const sf::Vector3f &other);
	
private:
	float matrix[4][4];
};

