#pragma once

#include <vector>

class Matrix {
public:
	Matrix(size_t n, size_t m);
	Matrix(const Matrix& other) : mat(other.mat) {}
	~Matrix() {}

private:
	std::vector<std::vector<double>> mat;
};

// ---- Inline implementation ----
inline Matrix::Matrix(size_t n, size_t m)
{
	mat.resize(n);
	for (size_t i = 0; i < n; i++)
	{
		mat[i].resize(m);
	}
}
