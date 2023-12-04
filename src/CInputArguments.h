#pragma once

#include <string>

struct CInputArguments
{
	CInputArguments( const unsigned int& aArgsCount, const char** aArgs );

	unsigned long mMaxSteps;
	unsigned long mSeed;
};