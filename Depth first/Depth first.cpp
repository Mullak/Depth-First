// Games Dev.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <deque>
#include <iostream>
#include <fstream>
using namespace std;

bool isFound = false;
const int SIZE = 10;
const int MAXCOORDS = 4;
// this structure is used to store the info for the nodes
struct Coords
{
	int x;
	int y;
	Coords* parent;
	int score;
};
// this is used to store the cost of a set of coordinates
struct num
{
	char Array[SIZE];
};

void SetStartEnd(string filename, Coords* begin, Coords* finish);

void Search(Coords* current, deque <Coords*> closedList, deque <Coords*> open, Coords* tmp);

void Program();

void CalculateScore(string filename, Coords* tmp, num cost[], Coords* current);

Coords* NewNorth(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[]);

Coords* NewEast(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[]);

Coords* NewSouth(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[]);

Coords* NewWest(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[]);

// this the main program and loop
void Program()
{
	Coords* start = new (Coords);
	Coords* end = new (Coords);
	Coords* current = new (Coords);
	Coords* tmp = new (Coords);
	num cost[SIZE];
	string mapFile;
	string coordsFile;
	char answer;
	bool NA[MAXCOORDS] = {false,false,false,false};
	// asks the user what map they want
	cout << "would you like map d or m" << endl;
	cin >> answer;
	if(answer == 'm')
	{
		mapFile = "..\\Debug\\mMap.txt";
		coordsFile = "..\\Debug\\mCoords.txt";
	}
	else if( answer == 'd')
	{
		mapFile = "..\\Debug\\dMap.txt";
		coordsFile = "..\\Debug\\dCoords.txt";
	}
	else
	{
		cout << "invaild chose" << endl;

	}
	deque <Coords*> openList;
	deque <Coords*> closedList;
	SetStartEnd(coordsFile, start, end);
	CalculateScore(mapFile, start,  cost, current);
	start->parent = 0;
	openList.push_back(start);
	ifstream infile;
	infile.open(mapFile);
	//sets up cost array
	for(int i = SIZE-1; i >= 0 ; i--)
	{
		for(int j = 0; j < SIZE; j++)
		{
			infile >> cost[i].Array[j];
			if(infile.eof())
			{
			
			}
		}
	}
	if(openList.empty())
	{
		cout << "Failure" << endl;
		return;
	}
	else
	{
		// main loop
		while(!openList.empty())
		{
			current = openList.back();
			cout <<"Current: "<< current->x << current->y << endl;
			cout<< endl;
			cout <<"ClosedList: " << endl;
			for(int i = 0; i < closedList.size(); i++)
			{
				cout << closedList[i]->x << closedList[i]->y << " ";
			}
			cout << endl;
			openList.pop_back();
			cout << "OpenList: " << endl;
			for(int i = 0; i < openList.size(); i++)
			{
				cout << openList[i]->x << openList[i]->y << " ";
			}
			cout << endl;
			if(current->x == end->x && current->y == end->y)
			{
				//file output if goal is found
				cout << "Goal Found" << endl;
				ofstream outfile;
				outfile.open("..//Debug//outfile.txt");
				outfile << current->x << current->y << " ";
				for(; ;)
				{
					
					if(current->parent == 0)
					{
						return;
					}
					else
					{
						outfile << current->parent->x << current->parent->y << " ";
						current = current->parent;
					}
					
				}
				delete(current, end, start);
				outfile.close();
				return;
			}
			closedList.push_back(current);
			tmp = NewNorth(current, closedList, openList, mapFile, cost);
			if(isFound)
			{
				NA[0] = true;
			}
			else
			{
				if(tmp->x < 0 || tmp->y < 0 || tmp->score <= 0 || tmp->x > 9 || tmp->y > 9)
				{
					NA[0] = true;
				}
				else
				{
					openList.push_back(tmp);
					NA[0] = false;
				}
			}
			tmp = NewEast(current, closedList, openList, mapFile, cost);
			if(isFound)
			{
				NA[1] = true;
			}
			else
			{
				if(tmp->x < 0 || tmp->y < 0  || tmp->score <= 0 || tmp->x > 9 || tmp->y > 9)
				{
					NA[1] = true;
				}
				else
				{
					openList.push_back(tmp);
					NA[1] = false;
				}
			}
			tmp = NewSouth(current, closedList, openList, mapFile, cost);
			if(isFound)
			{
				NA[2] = true;
			}
			else
			{
				if(tmp->x < 0 || tmp->y < 0 || tmp->score <= 0 || tmp->x > 9 || tmp->y > 9)
				{
					NA[2] = true;
				}
				else
				{
					openList.push_back(tmp);
					NA[2] = false;
				}
			}

			tmp = NewWest(current, closedList, openList, mapFile, cost);
			if(isFound)
			{
				NA[3] = true;
			}
			else
			{
				if(tmp->x < 0 || tmp->y < 0 || tmp->score <= 0 || tmp->x > 9 || tmp->y > 9)
				{
					NA[3] = true;
				}
				else
				{
					openList.push_front(tmp);
					NA[3] = false;
				}
			}
			if(openList.empty())
			{
				return;
			}
			if(NA[0] == true && NA[1] == true && NA[2] == true && NA[3] == true)
			{
				current = current->parent;
			}
		}
	}
	delete(current, end, start);
}
// this sets the new nodes score
void CalculateScore(string filename, Coords* tmp, num cost[], Coords* current)
{
	tmp->score = cost[tmp->x].Array[tmp->y];
}

//the four functions below generate the new nodes
Coords* NewNorth(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[])
{
	Coords* tmp = new Coords;
	tmp->x = current->x;
	tmp->y = current->y +1;
	tmp->parent = current;
	CalculateScore(mapFile, tmp, cost, current);
	Search(current, closedList, openList, tmp);
	return tmp;
}

Coords* NewEast(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[])
{
	Coords* tmp = new Coords;
	tmp->x = current->x +1;
	tmp->y = current->y;
	tmp->parent = current;
	CalculateScore(mapFile, tmp, cost, current);
	Search(current, closedList, openList, tmp);
	return tmp;
}

Coords* NewSouth(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[])
{
	Coords* tmp = new Coords;
	tmp->x = current->x;
	tmp->y = current->y -1;
	tmp->parent = current;
	CalculateScore(mapFile, tmp, cost, current);
	Search(current, closedList, openList, tmp);
	return tmp;
}

Coords* NewWest(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, string mapFile, num cost[])
{
	Coords* tmp = new Coords;
	tmp->x = current->x -1;
	tmp->y = current->y;
	tmp->parent = current;
	CalculateScore(mapFile, tmp, cost, current);
	Search(current, closedList, openList, tmp);
	return tmp;
}
// this searches the open and closed list to see it finds the new node it one of them if so sets isfoind to true
void Search(Coords* current, deque <Coords*> closedList, deque <Coords*> openList, Coords* tmp)
{
	isFound = false;
	deque <Coords*>::iterator it;
	if(!closedList.empty())
	{
		for(it = closedList.begin(); it != closedList.end(); it++)
		{
			if((*it)->x == tmp->x && (*it)->y == tmp->y)
			{
				isFound = true;
				return;
			}
		}
		for(it = openList.begin(); it != openList.end(); it++)
		{
			if((*it)->x == tmp->x && (*it)->y == tmp->y)
			{
				isFound = true;
				return;
			}
		}
	}
	else
	{
		cout << "Closed List is empty" << endl;
	
	}
}
//this function sets the start and end nodes up
void SetStartEnd(string filename, Coords* begin, Coords* finish)
{
	ifstream infile;
	infile.open(filename);
	
	if(infile.is_open())
	{
		infile >> begin->x; 
		infile >> begin->y; 
		infile >> finish->x; 
		infile >> finish->y;
	}
	else
	{
		cout << "ERROR opening input file" << endl;
	}
	infile.close();
}

int _tmain(int argc, _TCHAR* argv[])
{
	Program();
	system("pause");
	return 0;
}

