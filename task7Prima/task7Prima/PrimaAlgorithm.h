#pragma once
#include <vector>
#include <thread>

#include "Matrix.h"

using namespace std;

class PrimaAlgorithm
{
private:

	bool contains(size_t* array, size_t size, size_t value)
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

	void prepareRersult(Matrix<float>& result)
	{
		Matrix<float>::fill(result, std::numeric_limits<float>::infinity());
		for (size_t i = 0; i < result.rows(); ++i)
		{
			result.at(i, i) = 0;
		}
	}

	void shortestPath(size_t startVertex, size_t* visitedVertices, size_t nVisitedVertices, size_t& endVertex, size_t from, size_t count)
	{
		float minValue = numeric_limits<float>::infinity();
		endVertex = -1;
		for (size_t vertex = from; vertex < from + count; ++vertex)
		{
			if (startVertex != vertex && !contains(visitedVertices, nVisitedVertices, vertex) && graph->at(startVertex, vertex) < minValue)
			{
				endVertex = vertex;
				minValue = graph->at(startVertex, endVertex);
			}
		}
	}

	void nextSmallestEdge(size_t* visitedVertices, size_t nVisitedVertices, size_t& startVertex, size_t& endVertex)
	{
		float minValue = numeric_limits<float>::infinity();
		for (size_t i = 0; i < nVisitedVertices; ++i)
		{
			for (size_t j = 0; j < nVertices; ++j)
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

	size_t minValid(size_t* array, size_t size)
	{
		size_t minValidIndex = -1;
		for (size_t i = 0; i < size; ++i)
		{
			if (array[i] != -1)
			{
				minValidIndex = i;
			}
		}

		if (minValidIndex != -1)
		{
			size_t result = array[minValidIndex];
			for (size_t i = minValidIndex + 1; i < size; ++i)
			{
				if (array[i] != -1 && array[i] < result)
				{
					result = array[i];
				}
			}
			return result;
		}
		else
		{
			return -1;
		}
	}

	void nextSmallestEdgeMT(size_t* visitedVertices, size_t nVisitedVertices, size_t& startVertex, size_t& endVertex, size_t nThreads)
	{
		size_t intervalLen = nVertices / nThreads;
		size_t restLen = nVertices - intervalLen * nThreads;

		size_t minValue = numeric_limits<float>::infinity();
		for (size_t i = 0; i < nVisitedVertices; ++i)
		{
			size_t* partResults = new size_t[nThreads];
			vector<thread> threads;
			for (size_t j = 0; j < nThreads; ++j)
			{
				size_t count = j < nThreads - 1 ? intervalLen : intervalLen + restLen;
				threads.push_back(thread(&PrimaAlgorithm::shortestPath, *this, visitedVertices[i], visitedVertices, nVisitedVertices, ref(partResults[j]), j * intervalLen, count));
			}

			for (size_t j = 0; j < nThreads; ++j)
			{
				threads[j].join();
			}

			size_t closestVertex = minValid(partResults, nThreads);

			if (closestVertex != -1 && graph->at(visitedVertices[i], closestVertex) < minValue)
			{
				minValue = graph->at(visitedVertices[i], closestVertex);
				startVertex = visitedVertices[i];
				endVertex = closestVertex;
				cout << startVertex + 1 << "-" << endVertex + 1 << " ";
			}
		}
	}

	Matrix<float>* graph;
	Matrix<float>* result;
	size_t nVertices;

public:

	PrimaAlgorithm(Matrix<float>& graph, Matrix<float>& result)
	{
		this->graph = &graph;
		this->result = &result;
		nVertices = graph.rows();
	}

	void run()
	{
		prepareRersult(*result);

		size_t* visitedVertices = new size_t[nVertices];
		visitedVertices[0] = 0;
		for (size_t i = 1; i < nVertices; ++i)
		{
			visitedVertices[i] = -1;
		}

		size_t nVisitedVertices = 1;
		
		while (nVisitedVertices < nVertices)
		{
			size_t startVertex = -1;
			size_t endVertex = -1;
			nextSmallestEdge(visitedVertices, nVisitedVertices, startVertex, endVertex);

			visitedVertices[nVisitedVertices] = endVertex;
			++nVisitedVertices;
			result->at(startVertex, endVertex) = graph->at(startVertex, endVertex);
		}
	}
};