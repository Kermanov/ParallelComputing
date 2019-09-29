#include <iostream>
#include <thread>
#include <ctime>
#include <vector>
#include <functional>

#include "ConsoleApp.h"
#include "Matrix.h"
#include "Matrix.cpp"

using namespace std;

void splitProcess(size_t nThreads,
	const Matrix<int>& mat1,
	const Matrix<int>& mat2,
	Matrix<int>& result,
	function<void(const Matrix<int>&, const Matrix<int>&, Matrix<int>&, size_t, size_t)> func
)
{
	size_t intervalLen = result.length() / nThreads;
	size_t restLen = result.length() % intervalLen;

	vector<thread> threads;
	cout << "started  | threads: " << nThreads << endl;
	size_t startTime = clock();
	for (size_t i = 0; i < nThreads; ++i)
	{
		size_t count = i < nThreads - 1 ? intervalLen : intervalLen + restLen;
		threads.push_back(thread(func, ref(mat1), ref(mat2), ref(result), i * intervalLen, count));
	}

	for (size_t i = 0; i < nThreads; ++i)
	{
		threads[i].join();
	}
	size_t finishTime = clock();

	cout << "finished | ";
	cout << "time: " << (finishTime - startTime) / 1000. << "s" << endl << endl;
}

void runTest(function<void(const Matrix<int>&, const Matrix<int>&, Matrix<int>&, size_t, size_t)> func)
{
	cout << "Enter rows and columns number: ";
	size_t size;
	cin >> size;

	cout << "Matrices creation..." << endl;
	Matrix<int> mat1(size);
	Matrix<int>::fillRandInt(mat1);
	Matrix<int> mat2(size);
	Matrix<int>::fillRandInt(mat2);
	Matrix<int> result(size);
	cout << "Matrices created." << endl << endl;

	while (true)
	{
		cout << "Enter number of threads (0 to exit): ";
		size_t nThreads;
		cin >> nThreads;

		if (nThreads == 0)
		{
			break;
		}

		splitProcess(nThreads, mat1, mat2, result, func);
	}
}

void addTest()
{
	runTest(addPart<int>);
}

void mulTest()
{
	runTest(mulPart<int>);
}

int main()
{
	vector<Function> funcs =
	{
		Function("add", "matrices adding test", addTest),
		Function("mul", "matrices mul. test", mulTest)
	};

	ConsoleApp app("Matrix Multithread", funcs);
	app.run();

	return 0;
}
