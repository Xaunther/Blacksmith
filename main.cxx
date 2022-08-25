// Main function

#include "SaveHistory.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

tableau::destination InputOrigin();
int main();

int main()
{
	// Initialize random seed
	srand( static_cast< unsigned int >( time( NULL ) ) );

	history best_poshistory;
	history::value_type epoch = InputOrigin();

	tableau best_board; // Initialize board (tableau)
	const tableau initial_board( "board.dat" );
	int N_pieces = initial_board.CountPieces(); // Count how many pieces there are (max number of steps one can achieve)

	for ( int i = 0; i < maxsteps && best_poshistory.size() < N_pieces; i++ ) // Iterate many times
	{
		tableau board{ initial_board };
		history poshistory = { epoch };
		// Randomize position if requested
		if ( poshistory.back().first == row || poshistory.back().second == col )
			poshistory.back() = initial_board.Randomize( poshistory.back() );
		while ( poshistory.back().first > -1 ) // When no moves available, it is -1
			poshistory.emplace_back( board.Move( poshistory.back() ) );
		poshistory.pop_back();
		if ( poshistory.size() > best_poshistory.size() ) // New record!
		{
			best_board = std::move( board );
			best_poshistory = std::move( poshistory );
		}
	}
	SaveHistory( best_poshistory, best_board, initial_board );
	return 0;
}

tableau::destination InputOrigin()
{
	tableau::destination result;
	// Ask for initial position
	std::cout << "Initial row? (Number 0-" << row - 1 << ") (" << row << " for random): ";
	std::cin >> result.first;
	std::cout << "Initial column? (Number 0-" << col - 1 << ") (" << col << " for random): ";
	std::cin >> result.second;
	return result;
}