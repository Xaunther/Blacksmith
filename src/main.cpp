#include "CInputArguments.h"
#include "CResult.h"

#include <iostream>
#include <future>

using namespace blacksmith;

int main( const unsigned int& aArgsCount, const char** aArgs )
{
	const CInputArguments inputArgs{ aArgsCount, aArgs };
	std::mt19937_64 RNG{ inputArgs.mSeed };

	const CTableau initialBoard( inputArgs.mBoardFileName );

	CResult bestResult{ CTableauState{ initialBoard, inputArgs.mInitialCoordinates } };
	std::async( &CResult::FindBest, &bestResult, inputArgs.mMaxSteps, std::ref( RNG ), inputArgs.mSpeed ).wait();
	bestResult.SaveHistory( inputArgs.mBestPatternFileName, initialBoard );

	return 0;
}
