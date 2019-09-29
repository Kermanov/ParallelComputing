#include <iostream>
#include <fstream>
#include <string>

#include "Matrix.h"
#include "Matrix.cpp"

#include "ConsoleApp.h"

using namespace std;

void calcPart(	const Matrix<double>& coef, 
				const Matrix<double>& bVect, 
				const Matrix<double>& result,
				Matrix<double>& tempResult,
				size_t index, size_t count
			 )
{
	for (size_t i = index; i < index + count; ++i)
	{
		double sum = 0;
		for (size_t j = 0; j < coef.rows(); ++j)
		{
			if (j != i)
			{
				sum += coef.at(i, j) * result.at(j, 0);
			}
		}
		tempResult.at(i, 0) = (bVect.at(i, 0) - sum) / coef.at(i, i);
	}
}

void calculate(const Matrix<double>& coef, const Matrix<double>& bVect, Matrix<double>& result, size_t nThreads = 1, double eps = 0.01)
{
	Matrix<double> tempResult(result.rows(), result.cols());
	double delta;
	do
	{
		size_t intervalLen = coef.rows() / nThreads;
		size_t restLen = coef.rows() - intervalLen * nThreads;

		vector<thread> threads;
		for (size_t i = 0; i < nThreads; ++i)
		{
			size_t count = i < nThreads - 1 ? intervalLen : intervalLen + restLen;
			threads.push_back(thread(calcPart, ref(coef), ref(bVect), ref(result), ref(tempResult), i * intervalLen, count));
		}

		for (size_t i = 0; i < nThreads; ++i)
		{
			threads[i].join();
		}

		delta = fabs(tempResult.at(0, 0) - result.at(0, 0));
		for (size_t i = 0; i < result.rows(); ++i)
		{
			double newNorm = fabs(tempResult.at(i, 0) - result.at(i, 0));
			if (newNorm > delta)
			{
				delta = newNorm;
			}
		}
		result = tempResult;
	} while (delta > eps);
}

void runCalc()
{
	cout << "Enter system size: ";
	size_t systemSize;
	cin >> systemSize;

	cout << "Enter filename: ";
	string filename;
	cin >> filename;

	cout << "Matrices reading..." << endl;

	Matrix<double> coefMatr(systemSize);
	ifstream file(filename + ".txt");
	file >> coefMatr;
	file.close();

	Matrix<double> bVect(systemSize, 1);
	file.open(filename + "_fcol.txt");
	file >> bVect;
	file.close();

	cout << "Matrices ready!" << endl;

	while (true)
	{
		Matrix<double> result = bVect;

		cout << "\nEnter number of threads (0 for exit): ";
		size_t nThreads;
		cin >> nThreads;

		if (nThreads == 0)
		{
			break;
		}

		cout << "Enter epsilon: ";
		double eps;
		cin >> eps;

		cout << "Calculating..." << endl;

		size_t startTime = clock();
		calculate(coefMatr, bVect, result, nThreads, eps);
		size_t finishTime = clock();

		cout << "Done! Time: " << (finishTime - startTime) / 1000. << "s." << endl;

		ofstream output(filename + "_result.txt");
		output << result;
		output.close();
		cout << "Results in file " << filename << "_result.txt" << endl;
	}
}

int main()
{
	vector<Function> funcs =
	{
		Function("calc", "calculates the equation", runCalc)
	};

	ConsoleApp app("Equations system calc", funcs);
	app.run();

	return 0;
}
