#include "CInputArguments.h"
#include "CResult.h"

#include <iostream>
#include <future>

using namespace blacksmith;

int main( const int aArgsCount, const char** aArgs )
{
	const CInputArguments inputArgs{ aArgsCount, aArgs };
	if( !inputArgs.mRun ) return 0;

	std::mt19937_64 RNG{ inputArgs.mSeed };
	const CTableau initialBoard( inputArgs.mBoardFileName );

	CResult bestResult{ CTableauState{ inputArgs.mInitialCoordinates } };
	std::async( &CResult::FindBest, &bestResult, inputArgs.mMaxSteps, initialBoard, std::ref( RNG ), inputArgs.mSpeed ).wait();
	bestResult.GetTableauState().SaveHistory( inputArgs.mBestPatternFileName, initialBoard );

	return 0;
}
