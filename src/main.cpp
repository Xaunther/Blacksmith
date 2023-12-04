#include "CResult.h"
#include "Constants.h"

#include <iostream>
#include <future>

using namespace blacksmith;

namespace
{

std::optional<CTableauState::coordinates> InputOrigin( const CTableau::index& aRows );
bool InputSpeedOverScore();

}

int main()
{
	// Initialize random seed
	std::random_device rd;
	std::mt19937_64 RNG{ rd() };

	const CTableau initialBoard( "board.dat" );
	const auto& initialPosition = InputOrigin( initialBoard.GetRows() );
	const auto& speedOverScore = InputSpeedOverScore();

	CResult bestResult{ CTableauState{ initialBoard, initialPosition } };
	std::async( &CResult::FindBest, &bestResult, MAXSTEPS, std::ref( RNG ), speedOverScore ).wait();
	bestResult.SaveHistory( "Best-pattern.txt", initialBoard );

	return 0;
}

namespace
{

std::optional<CTableauState::coordinates> InputOrigin( const CTableau::index& aRows )
{
	CTableauState::coordinates result;
	// Ask for initial position
	std::cout << "Initial row? (Number 0-" << aRows - 1 << ") (" << aRows << " for random): ";
	std::cin >> result.first;
	std::cout << "Initial column? (Number 0-" << aRows - 1 << ") (" << aRows << " for random): ";
	std::cin >> result.second;
	return ( result.first < aRows && result.second < aRows ) ? result : std::optional<CTableauState::coordinates>{};
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