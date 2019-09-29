#pragma once
#include <iostream>

template<typename T>
class Matrix
{
	T** matrix;
	size_t nRow;
	size_t nCol;
public:
	size_t rows() const;
	size_t cols() const;
	size_t length() const;

	Matrix(size_t nRow, size_t nCol);
	Matrix(size_t nRowCol);
	Matrix(const Matrix& matr);
	~Matrix();

	T& at(size_t index);
	const T& at(size_t index) const;
	T& at(size_t row, size_t col);
	const T& at(size_t row, size_t col) const;

	static void fillRandInt(Matrix& matr, int min = 0, int max = 100);
	static void fill(Matrix& matr, T value);

	Matrix& operator=(const Matrix& right);

	template<typename U>
	friend std::ostream& operator<<(std::ostream& os, const Matrix<U>& matrix);
	template<typename U>
	friend std::istream& operator>>(std::istream& is, Matrix<U>& matrix);
};

template<typename T>
void addPart(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t index, size_t count);

template<typename T>
void mulPart(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t index, size_t count);

template<typename T>
void mulMT(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t nThreads = 1);