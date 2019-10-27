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

	void shortestPathPart(size_t startVertex, size_t* visited, size_t nVisited, float& minValue, size_t& endVertex, size_t& penultVertex, size_t from, size_t count)
	{
		for (size_t vertex = from; vertex < from + count; ++vertex)
		{
			if (startVertex != vertex && !contains(visited, nVisited, vertex) && graph->at(startVertex, vertex) < minValue)
			{
				endVertex = vertex;
				minValue = graph->at(startVertex, vertex);
				penultVertex = startVertex;
			}
		}
	}

	void shortestPath(size_t startVertex, size_t* visited, size_t nVisited, float& minValue, size_t& endVertex, size_t& penultVertex)
	{
		shortestPathPart(startVertex, visited, nVisited, minValue, endVertex, penultVertex, 0, nVertices);
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
			size_t nextVertex;
			float minValue = std::numeric_limits<float>::infinity();
			size_t penultVertex;
			for (size_t i = 0; i < nVisitedVertices; ++i)
			{
				shortestPath(visitedVertices[i], visitedVertices, nVisitedVertices, minValue, nextVertex, penultVertex);
			}

			visitedVertices[nVisitedVertices] = nextVertex;
			++nVisitedVertices;
			result->at(penultVertex, nextVertex) = minValue;
		}
	}

	void runMultiThread(size_t nThreads)
	{
		prepareRersult(*result);

		size_t* visitedVertices = new size_t[nVertices];
		visitedVertices[0] = 0;
		for (size_t i = 1; i < nVertices; ++i)
		{
			visitedVertices[i] = -1;
		}

		size_t nVisitedVertices = 1;

		size_t intervalLen = nVertices / nThreads;
		size_t restLen = nVertices - intervalLen * nThreads;

		while (nVisitedVertices < nVertices)
		{
			size_t nextVertex;
			float minValue = std::numeric_limits<float>::infinity();
			size_t penultVertex;
			for (size_t i = 0; i < nVisitedVertices; ++i)
			{
				shortestPath(visitedVertices[i], visitedVertices, nVisitedVertices, minValue, nextVertex, penultVertex);
			}

			visitedVertices[nVisitedVertices] = nextVertex;
			++nVisitedVertices;
			result->at(penultVertex, nextVertex) = minValue;
		}
	}
};