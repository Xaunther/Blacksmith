// Main function

#include "SaveHistory.h"
#include "result.h"
#include <iostream>
#include <time.h>

using namespace blacksmith;

tableau::destination InputOrigin();
result FindBestMoves( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries );
int main();

int main()
{
	// Initialize random seed
	srand( static_cast< unsigned int >( time( NULL ) ) );

	const auto& initialPosition = InputOrigin();
	const tableau initialBoard( "board.dat" );

	result bestResult;
	bestResult.FindBest( initialBoard, initialPosition, maxsteps );

	SaveHistory( bestResult.GetHistory(), bestResult.GetTableau(), initialBoard );
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