#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

class Matrix
{
public:
	Matrix(float initValue = 0);
	~Matrix();

	void setItem(int x, int y, float value);
	float getItem(int x, int y) const;
	void showItems();

	Matrix operator *(const Matrix &other);
	sf::Vector3f operator *(const sf::Vector3f &other);

private:
	float** matrix = nullptr;
};

