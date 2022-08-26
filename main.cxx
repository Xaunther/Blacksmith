// Main function

#include "SaveHistory.h"
#include <iostream>
#include <time.h>

using result = std::pair<history, tableau>;

tableau::destination InputOrigin();
result FindBestMoves( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries );
int main();

int main()
{
	// Initialize random seed
	srand( static_cast< unsigned int >( time( NULL ) ) );

	const auto& initialPosition = InputOrigin();
	const tableau initialBoard( "board.dat" );

	const auto& result = FindBestMoves( initialBoard, initialPosition, maxsteps );

	SaveHistory( result.first, result.second, initialBoard );
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

result FindBestMoves( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries )
{
	result bestMoves;
	const int N_pieces = aInitialBoard.CountPieces(); // Count how many pieces there are (max number of steps one can achieve)

	for( unsigned int i = 0; i < aNTries && bestMoves.first.size() < N_pieces; i++ ) // Iterate many times
	{
		history poshistory = { aInitialPosition };
		tableau board{ aInitialBoard };
		// Randomize position if requested
		if( poshistory.back().first == row || poshistory.back().second == col )
			poshistory.back() = aInitialBoard.Randomize( poshistory.back() );
		while( poshistory.back().first > -1 ) // When no moves available, it is -1
			poshistory.emplace_back( board.Move( poshistory.back() ) );
		poshistory.pop_back();
		if( poshistory.size() > bestMoves.first.size() ) // New record!
		{
			bestMoves.first = std::move( poshistory );
			bestMoves.second = std::move( board );
		}
	}

	return bestMoves;
}