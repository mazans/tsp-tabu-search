#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <chrono>
#include "Graph.h"

using namespace std;

static Graph graph;
static int timeInSeconds;
static string file_name = "";

void readGraphFromFile();
void setStopCriterion();
void executeTabuSearch();

int main() {

	char choice;

	srand(time(NULL));

	while (true) {
		cout << "Nazwa otwartego pliku: " << file_name << endl << endl;
		cout << "Kryterium stopu: " << timeInSeconds << " sekund" << endl << endl;
		cout << "Wybierz opcje:" << endl;
		cout << "1. Pobierz graf z pliku" << endl;
		cout << "2. Wprowadz kryterium stopu" << endl;
		cout << "3. Wykonaj przeszukiwanie Tabu" << endl;
		cout << "X. Wyjdz" << endl;
		cin >> choice;
		if (choice == '1')
			readGraphFromFile();
		else if (choice == '2')
			setStopCriterion();
		else if (choice == '3')
			executeTabuSearch();
		else if (choice == 'X')
			break;
		system("cls");
	}
	return 0;
}

void readGraphFromFile() 
{
	
	cout << "Podaj nazwe pliku z ktorego maja zostac pobrane dane: ";
	cin >> file_name;
	ifstream fin(file_name.c_str());

	int size;

	if (fin.is_open())
	{
		fin >> size;
		if (fin.fail())
		{
			cout << "Blad odczytu rozmiaru struktury!" << endl;;
			cin.get();
			cin.get();
			file_name = "";
			return;
		}
		else
		{
			graph = Graph(size);
			int val;
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					fin >> val;
					if (fin.fail())
					{
						cout << "Blad w trakcie odczytu danych ze struktury!";
						cin.get();
						cin.get();
						fin.close();
						graph = Graph();
						file_name = "";
						return;
					}
					else
					{
						graph.setEdgeValue(i, j, val);
					}
				}
			}
			fin.close();
		}
	}
	else
	{
		cout << "Blad w trakcie otwierania pliku!" << endl;
		cin.get();
		cin.get();
		file_name = "";
	}
}

void setStopCriterion()
{
	cout << "Podaj czas wykonywania sie algorytmu w sekundach: ";
	cin >> timeInSeconds;
}

void executeTabuSearch()
{
	graph.findResultUsingTabuSearch(timeInSeconds);
	graph.showResult();
	cin.get();
	cin.get();
}