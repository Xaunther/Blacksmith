// Implementation of tableau

#include "tableau.h"
#include "destination.h"
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;

// Initializer
tableau::tableau()
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			pieces[i][j] = "E";
		}
	}
}

// Load "map" from file, file must follow the codes
void tableau::Load(string filename)
{
	ifstream infile;
	string basura;
	infile.open(filename.c_str());
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			infile >> pieces[i][j];
		}
	}
	infile.close();
}

// Move from current place to another suitable one, randomly. Return next point
destination tableau::Move(int x, int y)
{
	int N_possible = 0;				  // Track number of possibilities
	destination destinations[maxpos]; // Save all possible destinations
	int dest_number;				  // Random destination number
	if (pieces[x][y] == "Q")		  // Queen
	{
		MoveDiagonal(x, y, destinations, N_possible);
		MoveStraight(x, y, destinations, N_possible);
	}
	else if (pieces[x][y] == "B") // Bishop
	{
		MoveDiagonal(x, y, destinations, N_possible);
	}
	else if (pieces[x][y] == "R") // Rook
	{
		MoveStraight(x, y, destinations, N_possible);
	}
	else if (pieces[x][y] == "K") // Knight
	{
		MoveKnight(x, y, destinations, N_possible);
	}
	else if (pieces[x][y] == "1") // 1
	{
		MoveDiagonal(x, y, destinations, N_possible, 1);
		MoveStraight(x, y, destinations, N_possible, 1);
	}
	else if (pieces[x][y] == "2") // 2
	{
		MoveDiagonal(x, y, destinations, N_possible, 2);
		MoveStraight(x, y, destinations, N_possible, 2);
	}
	else if (pieces[x][y] == "3") // 3
	{
		MoveDiagonal(x, y, destinations, N_possible, 3);
		MoveStraight(x, y, destinations, N_possible, 3);
	}
	else if (pieces[x][y] == "4") // 4
	{
		MoveDiagonal(x, y, destinations, N_possible, 4);
		MoveStraight(x, y, destinations, N_possible, 4);
	}
	else if (pieces[x][y] == "W") // Rum (WILDCARD!)
	{
		MoveWildcard(x, y, destinations, N_possible);
	}
	if (N_possible > 0) // If it's possible
	{
		dest_number = rand() % N_possible;
	}
	else
	{
		dest_number = 0;
		destinations[dest_number].x = -1;
		destinations[dest_number].y = -1;
	}
	pieces[x][y] = "E"; // Remove the piece, and make it empty
	return destinations[dest_number];
}

void tableau::MoveDiagonal(int x, int y, destination destinations[maxpos], int &N_possible, int steps)
{
	// Junk destination variable to test before adding
	destination test_dest[movepos];
	// 4 possible moves
	int i = 0;
	if (steps < 0)
	{
		test_dest[i].x = x - min(x - 0, y - 0);
		test_dest[i].y = y - min(x - 0, y - 0);
		i++;
		test_dest[i].x = x + min(5 - x, y - 0);
		test_dest[i].y = y - min(5 - x, y - 0);
		i++;
		test_dest[i].x = x + min(5 - x, 5 - y);
		test_dest[i].y = y + min(5 - x, 5 - y);
		i++;
		test_dest[i].x = x - min(x - 0, 5 - y);
		test_dest[i].y = y + min(x - 0, 5 - y);
		i++;
	}
	else
	{
		test_dest[i].x = x - steps;
		test_dest[i].y = y - steps;
		i++;
		test_dest[i].x = x + steps;
		test_dest[i].y = y - steps;
		i++;
		test_dest[i].x = x + steps;
		test_dest[i].y = y + steps;
		i++;
		test_dest[i].x = x - steps;
		test_dest[i].y = y + steps;
		i++;
		// If off-limits, return to initial pos
		for (i = 0; i < movepos; i++)
		{
			if (test_dest[i].x < 0 || test_dest[i].x > 5 || test_dest[i].y < 0 || test_dest[i].y > 5)
			{
				test_dest[i].x = x;
				test_dest[i].y = y;
			}
		}
	}
	for (i = 0; i < movepos; i++)
	{
		if ((test_dest[i].x == x && test_dest[i].y == y) || pieces[test_dest[i].x][test_dest[i].y] == "E")
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[N_possible] = test_dest[i];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveStraight(int x, int y, destination destinations[maxpos], int &N_possible, int steps)
{
	// Junk destination variable to test before adding
	destination test_dest[movepos];
	// 4 possible moves
	int i = 0;
	if (steps < 0)
	{
		test_dest[i].x = 0;
		test_dest[i].y = y;
		i++;
		test_dest[i].x = 5;
		test_dest[i].y = y;
		i++;
		test_dest[i].x = x;
		test_dest[i].y = 0;
		i++;
		test_dest[i].x = x;
		test_dest[i].y = 5;
		i++;
	}
	else
	{
		test_dest[i].x = x + steps;
		test_dest[i].y = y;
		i++;
		test_dest[i].x = x - steps;
		test_dest[i].y = y;
		i++;
		test_dest[i].x = x;
		test_dest[i].y = y + steps;
		i++;
		test_dest[i].x = x;
		test_dest[i].y = y - steps;
		i++;
		// If off-limits, return to initial pos
		for (i = 0; i < movepos; i++)
		{
			if (test_dest[i].x < 0 || test_dest[i].x > 5 || test_dest[i].y < 0 || test_dest[i].y > 5)
			{
				test_dest[i].x = x;
				test_dest[i].y = y;
			}
		}
	}
	for (i = 0; i < movepos; i++)
	{
		if ((test_dest[i].x == x && test_dest[i].y == y) || pieces[test_dest[i].x][test_dest[i].y] == "E")
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[N_possible] = test_dest[i];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveKnight(int x, int y, destination destinations[maxpos], int &N_possible)
{
	// Junk destination variable to test before adding
	destination test_dest[movepos_K];
	// 4 possible moves
	int i = 0;
	test_dest[i].x = x - 1;
	test_dest[i].y = y - 2;
	i++;
	test_dest[i].x = x - 1;
	test_dest[i].y = y + 2;
	i++;
	test_dest[i].x = x + 1;
	test_dest[i].y = y - 2;
	i++;
	test_dest[i].x = x + 1;
	test_dest[i].y = y + 2;
	i++;
	test_dest[i].x = x - 2;
	test_dest[i].y = y - 1;
	i++;
	test_dest[i].x = x - 2;
	test_dest[i].y = y + 1;
	i++;
	test_dest[i].x = x + 2;
	test_dest[i].y = y - 1;
	i++;
	test_dest[i].x = x + 2;
	test_dest[i].y = y + 1;
	i++;

	// If off-limits, return to initial pos
	for (i = 0; i < movepos_K; i++)
	{
		if (test_dest[i].x < 0 || test_dest[i].x > 5 || test_dest[i].y < 0 || test_dest[i].y > 5)
		{
			test_dest[i].x = x;
			test_dest[i].y = y;
		}
	}
	for (i = 0; i < movepos_K; i++)
	{
		if ((test_dest[i].x == x && test_dest[i].y == y) || pieces[test_dest[i].x][test_dest[i].y] == "E")
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[N_possible] = test_dest[i];
			N_possible++;
		}
	}
	return;
}

void tableau::MoveWildcard(int x, int y, destination destinations[maxpos], int &N_possible)
{
	// Junk destination variable to test before adding
	destination test_dest[maxpos];
	// Wildcard, all are possible
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			test_dest[col * i + j].x = i;
			test_dest[col * i + j].y = j;
		}
	}
	for (int i = 0; i < maxpos; i++)
	{
		if ((test_dest[i].x == x && test_dest[i].y == y) || pieces[test_dest[i].x][test_dest[i].y] == "E")
		{
		} // If same place or empty place do nothing
		else
		{
			destinations[N_possible] = test_dest[i];
			N_possible++;
		}
	}
	return;
}

destination tableau::Randomize(int x, int y)
{
	int N_possible = 0;				  // Track number of possibilities
	destination destinations[maxpos]; // Save all possible destinations
	int dest_number;				  // Random destination number
	MoveWildcard(x, y, destinations, N_possible);
	if (N_possible > 0) // If it's possible
	{
		dest_number = rand() % N_possible;
	}
	else
	{
		dest_number = 0;
		destinations[dest_number].x = -1;
		destinations[dest_number].y = -1;
	}
	return destinations[dest_number];
}

int tableau::CountPieces()
{
	// Loop around and count non-empty slots
	int count = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			if (pieces[i][j] != "E")
			{
				count++;
			}
		}
	}
	return count;
}
