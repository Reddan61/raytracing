#include "Matrix3d.h"

Matrix3d::Matrix3d(float initValue)
{
	for (short y = 0; y < 3; y++) {
		for (short x = 0; x < 3; x++) {
			this->matrix[y][x] = initValue;
		}
	}

}

Matrix3d::Matrix3d(const Matrix3d& other)
{
	for (short y = 0; y < 3; y++) {
		for (short x = 0; x < 3; x++) {
			matrix[y][x] = other.getItem(y, x);
		}
	}
}

Matrix3d::~Matrix3d()
{

}

void Matrix3d::setItem(int y, int x, float value)
{
	this->matrix[y][x] = value;
}

float Matrix3d::getItem(int y, int x) const
{
	return this->matrix[y][x];
}

void Matrix3d::showItems()
{
	for (short y = 0; y < 3; y++) {
		for (short x = 0; x < 3; x++) {
			std::cout << this->getItem(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

Matrix3d Matrix3d::operator*(const Matrix3d& other)
{
	Matrix3d result;

	for (short y = 0; y < 3; y++) {
		for (short x = 0; x < 3; x++) {
			float sum = 0;

			for (short i = 0; i < 3; i++) {
				sum += this->getItem(y, i) * other.getItem(i, x);
			}

			result.setItem(x, y, sum);
		}

	}

	return result;
}

Vector3d Matrix3d::operator*(const Vector3d& other)
{
	float test[3] = { other.x, other.y, other.z };
	float result[3] = { 0, 0, 0 };

	for (short y = 0; y < 3; y++) {
		float sum = 0;

		for (short x = 0; x < 3; x++) {
			sum += test[x] * this->getItem(y, x);
		}

		result[y] = sum;
	}


	return Vector3d(result[0], result[1], result[2]);
}
