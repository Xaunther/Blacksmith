#include "CInputArguments.h"
#include "CResult.h"

#include <iostream>
#include <future>

using namespace blacksmith;

namespace
{

bool InputSpeedOverScore();

}

int main( const unsigned int& aArgsCount, const char** aArgs )
{
	const CInputArguments inputArgs{ aArgsCount, aArgs };
	std::mt19937_64 RNG{ inputArgs.mSeed };

	const CTableau initialBoard( inputArgs.mBoardFileName );
	const auto& speedOverScore = InputSpeedOverScore();

	CResult bestResult{ CTableauState{ initialBoard, inputArgs.mInitialCoordinates } };
	std::async( &CResult::FindBest, &bestResult, inputArgs.mMaxSteps, std::ref( RNG ), speedOverScore ).wait();
	bestResult.SaveHistory( inputArgs.mBestPatternFileName, initialBoard );

	return 0;
}

namespace
{

bool InputSpeedOverScore()
{
	std::string result;
	// Ask for speed or score
	std::cout << "Prefer speed over score? [y/N]: ";
	std::cin >> result;
	return result == "y" || result == "Y";
}

}