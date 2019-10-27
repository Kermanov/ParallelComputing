#include <iostream>
#include <fstream>

#include "PrimaAlgorithm.h"
#include "Matrix.h"
#include "Matrix.cpp"
using namespace std;

int main()
{
	size_t nVertices = 6;
	Matrix<float> graph(nVertices);
	ifstream fileInput("graph.txt");
	fileInput >> graph;
	fileInput.close();

	Matrix<float> result(nVertices);
	
	PrimaAlgorithm alg(graph, result);
	alg.run();

	ofstream fileOutput("result.txt");
	fileOutput << result;
	fileOutput.close();

	return 0;
}