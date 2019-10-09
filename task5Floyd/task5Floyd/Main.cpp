#include <iostream>
#include <limits>
#include <thread>
#include <vector>
#include <fstream>

#include "Matrix.h"
#include "Matrix.cpp"
#include "ConsoleApp.h"

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

void runTask()
{
	cout << "Enter number of vertices: ";
	size_t nVertices;
	cin >> nVertices;

	cout << "Enter filename: ";
	string filename;
	cin >> filename;

	cout << "Matrix reading..." << endl;
	Matrix<float> matrix(nVertices);
	ifstream fin(filename);
	fin >> matrix;
	fin.close();
	cout << "Done." << endl;

	while (true)
	{
		cout << "\nEnter number of threads: ";
		size_t nThreads;
		cin >> nThreads;

		if (nThreads == 0)
		{
			break;
		}

		size_t startTime = clock();
		calc(matrix, nThreads);
		size_t finishTime = clock();

		cout << "Time: " << (finishTime - startTime) / 1000. << "s" << endl;
	}
	
	cout << "Saving result..." << endl;
	ofstream fout(filename.substr(0, filename.length() - 3) + "_result.txt");
	fout << matrix;
	fout.close();
	cout << "Done." << endl;
}

int main()
{
	vector<Function> funcs =
	{
		Function("run", "run task", runTask)
	};

	ConsoleApp app("Task 5", funcs);
	app.run();

	return 0;
}