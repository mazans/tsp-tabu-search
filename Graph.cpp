#include "pch.h"
#include <cmath>
#include <climits>
#include <list>
#include <iostream>
#include "Graph.h"
#include <chrono>
#include <stdlib.h>
#include <algorithm>
#include <random>

Graph::Graph()
{
	vertexesAmount = 1;
	cities = new int*[vertexesAmount];
	cities[0] = new int[vertexesAmount];
	cities[0][0] = 0;

	tabuList = new int*[vertexesAmount];
	tabuList[0] = new int[vertexesAmount];
	tabuList[0][0] = 0;
}

Graph::Graph(int vertexesAmount)
{
	this->vertexesAmount = vertexesAmount;
	cities = new int *[vertexesAmount];
	tabuList = new int *[vertexesAmount];
	for (int i = 0; i < vertexesAmount; i++) 
	{
		cities[i] = new int[vertexesAmount];
		tabuList[i] = new int[vertexesAmount];
		for (int j = 0; j < vertexesAmount; j++)
		{
			cities[i][j] = 0;
			tabuList[i][j] = 0;
		}
	}
	
	cadention = vertexesAmount;
}

Graph::Graph(const Graph & graph) 
{
	cadention = graph.cadention;
	vertexesAmount = graph.vertexesAmount;
	cities = new int *[vertexesAmount];
	tabuList = new int *[vertexesAmount];
	for (int i = 0; i < vertexesAmount; i++)
	{
		cities[i] = new int[vertexesAmount];
		tabuList[i] = new int[vertexesAmount];
		for (int j = 0; j < vertexesAmount; j++)
		{
			cities[i][j] = graph.cities[i][j];
			tabuList[i][j] = graph.tabuList[i][j];
		}
	}
}

Graph Graph::operator=(const Graph & graph) 
{
	cadention = graph.cadention;
	if (this == &graph)
		return *this;

	for (int i = 0; i < vertexesAmount; i++) 
	{
		delete[] cities[i];
		delete[] tabuList[i];
	}
	delete[] cities;
	delete[] tabuList;

	vertexesAmount = graph.vertexesAmount;
	cities = new int *[vertexesAmount];
	tabuList = new int *[vertexesAmount];
	for (int i = 0; i < vertexesAmount; i++)
	{
		cities[i] = new int[vertexesAmount];
		tabuList[i] = new int[vertexesAmount];
		for (int j = 0; j < vertexesAmount; j++)
		{
			cities[i][j] = graph.cities[i][j];
			tabuList[i][j] = graph.tabuList[i][j];
		}
	}

	return *this;
}

void Graph::setEdgeValue(int from, int to, int value) 
{
	cities[from][to] = value;
}

//metody dla Tabu Search
void Graph::findResultUsingTabuSearch(long long timeToStopInSeconds)
{
	typedef chrono::high_resolution_clock Clock;
	chrono::time_point<Clock> start, stop;

	clearTabuList();
	generateStartPoint();

	start = Clock::now();
	do
	{
		findTheBestNeighbourForTabuSearch();
		if (currentCost < optimalCost) 
		{
			optimalPath = currentPath;
			optimalCost = currentCost;
		}
		addTabu();
		updateTabuList();

		stop = Clock::now();
	} while (chrono::duration_cast<chrono::nanoseconds>(stop - start).count() < (timeToStopInSeconds * 1000000000));
}

void Graph::generateStartPoint()
{
	currentPath = vector<int>(vertexesAmount);

	for (int i = 0; i < vertexesAmount; i++)
	{
		currentPath[i] = i;
	}

	random_shuffle(begin(currentPath), end(currentPath));

	currentCost = calculateCost(currentPath);

	optimalPath = currentPath;
	optimalCost = currentCost;
}

int Graph::calculateCost(vector<int> path)
{
	int cost = 0;
	for (int i = 0; i < path.size() - 1; i++)
		cost = cost + cities[path[i]][path[i + 1]];
	cost = cost + cities[path[path.size() - 1]][path[0]];
	return cost;
}

void Graph::findTheBestNeighbourForTabuSearch()
{
	vector<int> tempPath = currentPath;
	int tempCost;

	vector<int> bestNeighbourPath;
	int bestNeighbourCost = INT_MAX;

	for (int i = 1; i < vertexesAmount - 1; i++)
	{
		for (int j = i + 1; j < vertexesAmount; j++)
		{
			swap(tempPath[i], tempPath[j]);
			tempCost = calculateCost(tempPath);
			if (aspiration(tempCost) || !isMoveTabu(i, j))
			{
				if (tempCost < bestNeighbourCost)
				{
					change.first = j;
					change.second = i;
					bestNeighbourPath = tempPath;
					bestNeighbourCost = tempCost;
				}
			}
			swap(tempPath[i], tempPath[j]);
		}
	}

	currentPath = bestNeighbourPath;
	currentCost = bestNeighbourCost;
}

bool Graph::aspiration(int cost)
{
	return cost < optimalCost;
}

bool Graph::isMoveTabu(int first, int second)
{
	return tabuList[first][second] != 0;
}

void Graph::addTabu()
{
	tabuList[change.first][change.second] = cadention;
	tabuList[change.second][change.first] = cadention;
}

void Graph::updateTabuList()
{
	for (int i = 0; i < vertexesAmount; i++)
	{
		for (int j = 0; j < vertexesAmount; j++)
		{
			if (tabuList[i][j] > 0)
				tabuList[i][j]--;
		}
	}
}

void Graph::clearTabuList()
{
	for (int i = 0; i < vertexesAmount; i++)
	{
		for (int j = 0; j < vertexesAmount; j++)
				tabuList[i][j] = 0;
	}
}

void Graph::showResult()
{
	cout << "Optymalna dlugosc sciezki: " << optimalCost << endl;
	cout << "Wierzcholki optymalnej scezki:" << endl;
	for (int vertix : optimalPath)
	{
		cout << "-" << vertix;
	}
	cout << endl;
}

Graph::~Graph() 
{
	for (int i = 0; i < vertexesAmount; i++)
	{
		delete[] cities[i];
		delete[] tabuList[i];
	}
	delete[] cities;
	delete[] tabuList;
}
