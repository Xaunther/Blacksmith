#include "CResult.h"
#include <iostream>
#include <thread>

using namespace blacksmith;

using threads = std::vector<std::thread>;

namespace
{

CTableau::destination InputOrigin();
bool InputSpeedOverScore();

}

int main()
{
	// Initialize random seed
	std::random_device rd;
	std::mt19937_64 RNG{ rd() };

	const auto& initialPosition = InputOrigin();
	const auto& speedOverScore = InputSpeedOverScore();
	const CTableau initialBoard( "board.dat" );

	CResult bestResult{ CTableauState{ initialBoard, initialPosition } };
	threads resultThreads;
	resultThreads.reserve( N_THREADS );
	for( unsigned short threadIndex = 0; threadIndex < N_THREADS; ++threadIndex )
		resultThreads.emplace_back( &CResult::FindBest, &bestResult, MAXSTEPS, std::ref( RNG ), speedOverScore );

	for( auto& resultThread : resultThreads )
		resultThread.join();
	bestResult.SaveHistory( "Best-pattern.txt", initialBoard );
	return 0;
}

namespace
{

CTableau::destination InputOrigin()
{
	CTableau::destination result;
	// Ask for initial position
	std::cout << "Initial row? (Number 0-" << ROW - 1 << ") (" << ROW << " for random): ";
	std::cin >> result.first;
	std::cout << "Initial column? (Number 0-" << COL - 1 << ") (" << COL << " for random): ";
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