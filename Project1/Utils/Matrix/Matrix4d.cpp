#include "Matrix4d.h"

Matrix4d::Matrix4d(float initValue)
{
	for (short y = 0; y < 4; y++) {
		for (short x = 0; x < 4; x++) {
			this->matrix[y][x] = initValue;
		}
	}
}

Matrix4d::~Matrix4d()
{
}

void Matrix4d::setItem(int y, int x, float value)
{
	this->matrix[y][x] = value;
}

float Matrix4d::getItem(int y, int x) const
{
	return this->matrix[y][x];
}

void Matrix4d::showItems()
{
	for (short y = 0; y < 4; y++) {
		for (short x = 0; x < 4; x++) {
			std::cout << this->getItem(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

Matrix4d Matrix4d::operator *(const Matrix4d& other)
{
	Matrix4d result;

	for (short y = 0; y < 4; y++) {
		for (short x = 0; x < 4; x++) {
			float sum = 0;

			for (short i = 0; i < 4; i++) {
				sum += this->getItem(y, i) * other.getItem(i, x);
			}

			result.setItem(x, y, sum);
		}

	}

	return result;
}

Vector3d Matrix4d::operator*(const Vector3d& other)
{
	Vector4d vec(other);
	Vector4d result;

	for (int y = 0; y < 4; y++) {
		float sum = 0;

		for (int x = 0; x < 4; x++) {
			sum += vec[x] * this->getItem(y, x);
		}

		result[y] = sum;
	}


	return Vector3d(result[0], result[1], result[2]);
}
