#pragma once
#include <vector>
#include <thread>

#include "Matrix.h"

using namespace std;

class PrimaAlgorithm
{
private:

	bool contains(int* array, size_t size, int value)
	{
		for (size_t i = 0; i < size; ++i)
		{
			if (array[i] == value)
			{
				return true;
			}
		}
		return false;
	}

	void nextSmallestEdge(int& startVertex, int& endVertex, size_t from, size_t count)
	{
		float minValue = numeric_limits<float>::infinity();
		for (size_t i = from; i < from + count; ++i)
		{
			for (int j = 0; j < nVertices; ++j)
			{
				if (visitedVertices[i] != j && !contains(visitedVertices, nVisitedVertices, j) && graph->at(visitedVertices[i], j) < minValue)
				{
					startVertex = visitedVertices[i];
					endVertex = j;
					minValue = graph->at(startVertex, endVertex);
				}
			}
		}
	}

	void nextSmallestEdgeMT(int& startVertex, int& endVertex, size_t maxThreads)
	{
		size_t verticesPerThread, restVertices, nThreads;
		if (nVisitedVertices >= maxThreads)
		{
			verticesPerThread = nVisitedVertices / maxThreads;
			restVertices = nVisitedVertices - verticesPerThread * maxThreads;
			nThreads = maxThreads;
		}
		else
		{
			verticesPerThread = 1;
			restVertices = 0;
			nThreads = nVisitedVertices;
		}

		thread* threads = new thread[nThreads];
		int* startPartResults = new int[nThreads];
		int* endPartResults = new int[nThreads];

		for (size_t i = 0; i < nThreads; ++i)
		{
			size_t count = i < nThreads - 1 ? verticesPerThread : verticesPerThread + restVertices;
			startPartResults[i] = -1;
			endPartResults[i] = -1;
			threads[i] = thread(&PrimaAlgorithm::nextSmallestEdge, *this, ref(startPartResults[i]), ref(endPartResults[i]), verticesPerThread * i, count);
		}

		for (size_t i = 0; i < nThreads; ++i)
		{
			threads[i].join();
		}

		float minValue = numeric_limits<float>::infinity();
		for (size_t i = 0; i < nThreads; ++i)
		{
			if (startPartResults[i] != -1 && endPartResults[i] != -1 && graph->at(startPartResults[i], endPartResults[i]) < minValue)
			{
				startVertex = startPartResults[i];
				endVertex = endPartResults[i];
				minValue = graph->at(startPartResults[i], endPartResults[i]);
			}
		}
	}

	void clear()
	{
		visitedVertices = new int[nVertices];
		visitedVertices[0] = 0;
		nVisitedVertices = 1;
	}

	const Matrix<float>* graph;
	Matrix<float>* result;
	size_t nVertices;
	int* visitedVertices;
	size_t nVisitedVertices;

public:

	PrimaAlgorithm(const Matrix<float>& graph, Matrix<float>& result)
	{
		this->graph = &graph;
		this->result = &result;

		Matrix<float>::fill(result, std::numeric_limits<float>::infinity());
		for (size_t i = 0; i < result.rows(); ++i)
		{
			result.at(i, i) = 0;
		}

		nVertices = graph.rows();
		clear();
	}

	void run()
	{
		while (nVisitedVertices < nVertices)
		{
			int startVertex, endVertex;
			nextSmallestEdge(startVertex, endVertex, 0, nVisitedVertices);

			visitedVertices[nVisitedVertices] = endVertex;
			++nVisitedVertices;
			result->at(startVertex, endVertex) = graph->at(startVertex, endVertex);
		}

		clear();
	}

	void runMultiThread(size_t nThreads)
	{
		while (nVisitedVertices < nVertices)
		{
			int startVertex, endVertex;
			nextSmallestEdgeMT(startVertex, endVertex, nThreads);

			visitedVertices[nVisitedVertices] = endVertex;
			++nVisitedVertices;
			result->at(startVertex, endVertex) = graph->at(startVertex, endVertex);
		}

		clear();
	}
};