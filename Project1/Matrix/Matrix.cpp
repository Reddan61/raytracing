#include "Matrix.h"

Matrix::Matrix(float initValue)
{
	float** matrix = new float* [3];

	for (short y = 0; y < 3; y++) {
		matrix[y] = new float[3];

		for (short x = 0; x < 3; x++) {
			matrix[y][x] = initValue;
		}
	}

	this->matrix = matrix;
}

Matrix::~Matrix()
{
	for (short y = 0; y < 3; y++) {
		delete this->matrix[y];
	}

	delete this->matrix;
}

void Matrix::setItem(int y, int x, float value)
{
	this->matrix[y][x] = value;
}

float Matrix::getItem(int y, int x) const
{
	return this->matrix[y][x];
}

void Matrix::showItems()
{
	for (short y = 0; y < 3; y++) {
		for (short x = 0; x < 3; x++) {
			std::cout << this->getItem(x, y) << " ";
		}
		std::cout << std::endl;
	}
}

Matrix Matrix::operator*(const Matrix& other)
{
	Matrix result;

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

sf::Vector3f Matrix::operator*(const sf::Vector3f& other)
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


	return sf::Vector3f(result[0], result[1], result[2]);
}
