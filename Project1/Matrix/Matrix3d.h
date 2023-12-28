#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Matrix3d
{
public:
	Matrix3d(float initValue = 0);
	Matrix3d(const Matrix3d &other);
	~Matrix3d();

	void setItem(int y, int x, float value);
	float getItem(int y, int x) const;
	void showItems();

	Matrix3d operator *(const Matrix3d &other);
	sf::Vector3f operator *(const sf::Vector3f &other);

private:
	float matrix[3][3];
};

