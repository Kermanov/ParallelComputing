#include "Matrix.h"

#include <thread>
#include <vector>
#include <string>

template<typename T>
inline size_t Matrix<T>::rows() const
{
	return nRow;
}

template<typename T>
inline size_t Matrix<T>::cols() const
{
	return nCol;
}

template<typename T>
inline size_t Matrix<T>::length() const
{
	return nRow * nCol;
}

template<typename T>
inline Matrix<T>::Matrix(size_t nRow, size_t nCol)
{
	this->nRow = nRow;
	this->nCol = nCol;

	matrix = new T* [nRow];
	for (size_t i = 0; i < nRow; ++i)
	{
		matrix[i] = new T[nCol];
	}
}

template<typename T>
inline Matrix<T>::Matrix(size_t nRowCol) : Matrix(nRowCol, nRowCol) {}

template<typename T>
inline Matrix<T>::Matrix(const Matrix<T>& matr): Matrix<T>(matr.nRow, matr.nCol)
{
	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t k = 0; k < nCol; ++k)
		{
			this->at(i, k) = matr.at(i, k);
		}
	}
}

template<typename T>
inline Matrix<T>::~Matrix()
{
	for (size_t i = 0; i < nRow; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;
}

template<typename T>
inline T& Matrix<T>::at(size_t index)
{
	return matrix[index / nCol][index % nCol];
}

template<typename T>
inline const T& Matrix<T>::at(size_t index) const
{
	return matrix[index / nCol][index % nCol];
}

template<typename T>
inline T& Matrix<T>::at(size_t row, size_t col)
{
	return matrix[row][col];
}

template<typename T>
inline const T& Matrix<T>::at(size_t row, size_t col) const
{
	return matrix[row][col];
}

template<typename T>
inline void Matrix<T>::fillRandInt(Matrix<T>& matr, int min, int max)
{
	if (max <= min)
	{
		throw std::exception("Max value must be greater then min value.");
	}

	for (size_t i = 0; i < matr.length(); ++i)
	{
		matr.at(i) = min + rand() % (max - min + 1);
	}
}

template<typename T>
inline void Matrix<T>::fill(Matrix& matr, T value)
{
	for (size_t i = 0; i < matr.rows(); ++i)
	{
		for (size_t k = 0; k < matr.cols(); ++k)
		{
			matr.at(i, k) = value;
		}
	}
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& right)
{
	if (this == &right) 
	{
		return *this;
	}

	for (size_t i = 0; i < nRow; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;

	nRow = right.nRow;
	nCol = right.nCol;

	matrix = new T*[nRow];
	for (size_t i = 0; i < nRow; ++i)
	{
		matrix[i] = new T[nCol];
	}

	for (size_t i = 0; i < nRow; ++i)
	{
		for (size_t k = 0; k < nCol; ++k)
		{
			matrix[i][k] = right.matrix[i][k];
		}
	}

	return *this;
}

template<typename T>
inline std::ostream& operator<<(std::ostream& os, const Matrix<T>& matrix)
{
	for (size_t i = 0; i < matrix.nRow; ++i)
	{
		for (size_t k = 0; k < matrix.nCol; ++k)
		{
			os << matrix.matrix[i][k] << '\t';
		}
		os << std::endl;
	}
	return os;
}

template<typename T>
inline std::istream& operator>>(std::istream& is, Matrix<T>& matrix)
{
	for (size_t i = 0; i < matrix.rows(); ++i)
	{
		for (size_t k = 0; k < matrix.cols(); ++k)
		{
			std::string input;
			is >> input;
			if (input == "inf")
			{
				matrix.at(i, k) = std::numeric_limits<float>::infinity();
			}
			else
			{
				matrix.at(i, k) = atof(input.c_str());
			}
		}
	}
	return is;
}

template<typename T>
inline void addPart(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t index, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		result.at(i) = left.at(index + i) + right.at(index + i);
	}
}

template<typename T>
inline void mulPart(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t index, size_t count)
{
	for (size_t i = 0; i < count; ++i)
	{
		size_t row = (i + index) / result.cols();
		size_t col = (i + index) % result.cols();

		T sum = 0;
		for (size_t k = 0; k < left.cols(); ++k)
		{
			sum += left.at(row, k) * right.at(k, col);
		}
		result.at(row, col) = sum;
	}
}

template<typename T>
inline void mulMT(const Matrix<T>& left, const Matrix<T>& right, Matrix<T>& result, size_t nThreads)
{
	size_t intervalLen = result.length() / nThreads;
	size_t restLen = result.length() % intervalLen;

	std::vector<std::thread> threads;
	for (size_t i = 0; i < nThreads; ++i)
	{
		size_t count = i < nThreads - 1 ? intervalLen : intervalLen + restLen;
		threads.push_back(std::thread(mulPart<T>, std::ref(left), std::ref(right), std::ref(result), i * intervalLen, count));
	}

	for (size_t i = 0; i < nThreads; ++i)
	{
		threads[i].join();
	}
}
