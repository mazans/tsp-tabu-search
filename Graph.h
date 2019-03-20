#ifndef PEA_2_GRAPH_H
#define PEA_2_GRAPH_H

#include <vector>
#include <list>
#include <utility>
#include <random>

using namespace std;

//Klasa reprezenujaca graf
class Graph 
{
private:
	//zmienne reprezentujace graf
	int vertexesAmount;
	int ** cities;

	//zmienne potrzebne do algorytmu Tabu Search
	vector<int> optimalPath;
	int optimalCost;

	vector<int> currentPath;
	int currentCost;

	//lista Tabu
	int cadention;
	int ** tabuList;

	pair<int, int> change;

	//meotdy potrzebne do wykonania algorytmu Tabu Search
	void generateStartPoint();
	int calculateCost(vector<int> path);

	void findTheBestNeighbourForTabuSearch();
	bool aspiration(int new_best);

	bool isMoveTabu(int first, int second);
	void addTabu();
	void updateTabuList();
	void clearTabuList();

public:
	Graph();
	Graph(int vertexesNumber);
	~Graph();

	Graph(const Graph & graph);
	Graph operator=(const Graph& graph);

	void setEdgeValue(int from, int to, int value);

	void findResultUsingTabuSearch(long long timeToStopInSeconds);

	void showResult();
};

#endif

