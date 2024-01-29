#pragma once
#include <iostream>
#include <vector>
#include "../Vector/Vector3d.h"

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
	Vector3d operator *(const Vector3d &other);

private:
	float matrix[3][3];
};

