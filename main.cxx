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

	history poshistory;
	history best_poshistory;

	int step = 0;
	int best_step = 0;

	tableau board; // Initialize board (tableau)
	tableau initial_board;
	std::string boarddata = "board.dat";				// File where initial board is saved
	initial_board.Load( boarddata );				// Load the board
	int N_pieces = initial_board.CountPieces(); // Count how many pieces there are (max number of steps one can achieve)

	for ( int i = 0; i < maxsteps; i++ ) // Iterate many times
	{
		board = initial_board;
		step = 0;
		// Randomize position if requested
		if ( poshistory[ step ].first == 6 || poshistory[ step ].second == 6 )
		{
			poshistory[ step ] = initial_board.Randomize( poshistory[ step ] );
		}
		while ( poshistory[ step ].first > -1 ) // When no moves available, it is -1
		{
			step++;
			poshistory[ step ] = board.Move( poshistory[ step - 1 ] );
		}
		if ( step > best_step ) // New record!
		{
			best_step = step;
			for ( int j = 0; j < best_step; j++ )
			{
				best_poshistory[ j ] = poshistory[ j ];
			}
			// End loop if maximum possible length is achieved (saves time)
			if ( best_step == N_pieces )
			{
				break;
			}
		}
	}
	SaveHistory( best_poshistory, best_step, initial_board );
	return 0;
}

tableau::destination InputOrigin()
{
	tableau::destination result;
	// Ask for initial position
	std::cout << "Initial row? (Number 0-5) (6 for random): ";
	std::cin >> result.first;
	std::cout << "Initial column? (Number 0-5) (6 for random): ";
	std::cin >> result.second;
	return result;
}