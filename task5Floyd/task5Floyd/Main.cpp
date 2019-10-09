#include <iostream>
#include <limits>
#include <thread>
#include <vector>
#include <fstream>

#include "Matrix.h"
#include "Matrix.cpp"

using namespace std;

template<typename T>
T min(T a, T b)
{
	if (a < b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

void calcPart(Matrix<float>& matrix, size_t k, size_t index, size_t count)
{
	for (size_t i = index; i < index + count; ++i)
	{
		size_t row = i / matrix.cols();
		size_t col = i % matrix.cols();

		matrix.at(row, col) = min(matrix.at(row, col), matrix.at(row, k) + matrix.at(k, col));
	}
}

void calc(Matrix<float>& matrix, size_t nThreads)
{
	size_t intervalLen = matrix.length() / nThreads;
	size_t restLen = matrix.length() - intervalLen * nThreads;

	for (size_t k = 0; k < matrix.cols(); ++k)
	{
		vector<thread> threads;
		for (size_t i = 0; i < nThreads; ++i)
		{
			size_t count = i < nThreads - 1 ? intervalLen : intervalLen + restLen;
			threads.push_back(thread(calcPart, ref(matrix), k, i * intervalLen, count));
		}

		for (size_t i = 0; i < nThreads; ++i)
		{
			threads[i].join();
		}
	}
}

void generateMatrix(Matrix<float>& matrix)
{
	for (size_t i = 0; i < matrix.cols(); ++i)
	{
		for (size_t j = 0; j < matrix.rows(); ++j)
		{
			if (i != j)
			{
				matrix.at(i, j) = 1 + (rand() % 20);
			}
			else
			{
				matrix.at(i, j) = 0;
			}
		}
	}
}

int main()
{
	size_t size = 1000;
	Matrix<float> matrix(size);

	generateMatrix(matrix);

	/*ifstream file("matrix.txt");
	file >> matrix;
	file.close();*/

	size_t nThreads = 2;

	size_t startTime = clock();
	calc(matrix, nThreads);
	size_t finishTime = clock();

	cout << (finishTime - startTime) / 1000. << endl;

	/*ofstream fout("result.txt");
	fout << matrix;
	fout.close();*/

	return 0;
}