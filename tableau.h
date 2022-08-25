// Header file for tableau class
#ifndef tableau_h
#define tableau_h

#include "Constants.h"
#include "destination.h"
#include <string>
using namespace std;

class tableau
{
public:
	tableau();
	void Load(string);
	destination Move(int, int);
	destination Randomize(int, int);
	void MoveDiagonal(int, int, destination[maxpos], int &, int steps = -1);
	void MoveStraight(int, int, destination[maxpos], int &, int steps = -1);
	void MoveKnight(int, int, destination[maxpos], int &);
	void MoveWildcard(int, int, destination[maxpos], int &);
	int CountPieces();

	// Variables
	string pieces[row][col]; // Codes for piece type: (E)mpty, (Q)ueen, (B)ishop, (R)ook, (K)night, 1, 2, 3, 4, (W)wildcard
};

#endif
