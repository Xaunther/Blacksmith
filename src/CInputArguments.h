#pragma once

#include <optional>
#include <string>

#include "CTableauState.h"

struct CInputArguments
{
	using coordinates = blacksmith::CTableauState::coordinates;

	CInputArguments( const unsigned int& aArgsCount, const char** aArgs );

	unsigned long mMaxSteps;
	unsigned long mSeed;
	std::string mBoardFileName;
	std::string mBestPatternFileName;
	std::optional<blacksmith::CTableauState::coordinates> mInitialCoordinates;
};