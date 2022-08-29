// Main function

#include "SaveHistory.h"
#include "result.h"
#include <iostream>
#include <thread>

using namespace blacksmith;

using threads = std::vector<std::thread>;

namespace
{

tableau::destination InputOrigin();
bool InputSpeedOverScore();

}

int main()
{
	// Initialize random seed
	std::random_device rd;
	std::mt19937_64 RNG{ rd() };

	const auto& initialPosition = InputOrigin();
	const auto& speedOverScore = InputSpeedOverScore();
	const tableau initialBoard( "board.dat" );

	result bestResult;
	threads resultThreads;
	resultThreads.reserve( N_threads );
	for( unsigned short threadIndex = 0; threadIndex < N_threads; ++threadIndex )
		resultThreads.emplace_back( &result::FindBest, &bestResult, initialBoard, initialPosition, maxsteps, std::ref( RNG ), speedOverScore );

	for( auto& resultThread : resultThreads )
		resultThread.join();
	SaveHistory( bestResult.GetHistory(), bestResult.GetTableau(), initialBoard );
	return 0;
}

namespace
{

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

bool InputSpeedOverScore()
{
	std::string result;
	// Ask for speed or score
	std::cout << "Prefer speed over score? [y/N]: ";
	std::cin >> result;
	return result == "y" || result == "Y";
}

}