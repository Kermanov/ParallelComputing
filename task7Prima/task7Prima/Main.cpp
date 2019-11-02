#include <iostream>
#include <fstream>

#include "PrimaAlgorithm.h"
#include "Matrix.h"
#include "Matrix.cpp"
#include "ConsoleApp.h"
using namespace std;

void runPrimaMultiThread()
{
	cout << "Enter number of vertices: ";
	size_t nVertices;
	cin >> nVertices;

	cout << "Enter filename: ";
	string filename;
	cin >> filename;

	cout << "Graph reading..." << endl;
	Matrix<float> graph(nVertices);
	ifstream fileInput(filename);
	fileInput >> graph;
	fileInput.close();
	cout << "Done." << endl;

	Matrix<float> result(nVertices);

	PrimaAlgorithm algorithm(graph, result);

	while (true)
	{
		cout << "\nEnter number of threads: ";
		size_t nThreads;
		cin >> nThreads;

		if (nThreads == 0)
		{
			break;
		}

		cout << "Calculating..." << endl;
		size_t startTime = clock();
		algorithm.runMultiThread(nThreads);
		size_t finishTime = clock();

		cout << "Time: " << (finishTime - startTime) / 1000. << "s" << endl;
	}

	cout << "Saving result..." << endl;
	ofstream fout(filename.substr(0, filename.length() - 4) + "_result.txt");
	fout << result;
	fout.close();
	cout << "Done." << endl;
}

void runPrima()
{
	cout << "Enter number of vertices: ";
	size_t nVertices;
	cin >> nVertices;

	cout << "Enter filename: ";
	string filename;
	cin >> filename;

	cout << "Graph reading..." << endl;
	Matrix<float> graph(nVertices);
	ifstream fileInput(filename);
	fileInput >> graph;
	fileInput.close();
	cout << "Done." << endl;

	Matrix<float> result(nVertices);

	PrimaAlgorithm algorithm(graph, result);
	cout << "Calculating..." << endl;
	size_t startTime = clock();
	algorithm.run();
	size_t finishTime = clock();

	cout << "Time: " << (finishTime - startTime) / 1000. << "s" << endl;

	cout << "Saving result..." << endl;
	ofstream fout(filename.substr(0, filename.length() - 4) + "_result.txt");
	fout << result;
	fout.close();
	cout << "Done." << endl;
}

int main()
{
	vector<Function> functions =
	{
		Function("run", "run Prima in main thread", runPrima),
		Function("runMT", "run Prima in multiply threads", runPrimaMultiThread)
	};

	ConsoleApp app("Prima Algorithm", functions);
	app.run();

	return 0;
}