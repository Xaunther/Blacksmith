#include "CInputArguments.h"

#include <sstream>

namespace
{

template <typename T> T GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue = T{} );

}

CInputArguments::CInputArguments( const unsigned int& aArgsCount, const char** aArgs ) :
	mMaxSteps( GetArgument<unsigned long>( aArgsCount, aArgs, "-n", 10000000 ) )
{
}

namespace
{

template <typename T> T GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue )
{
	const auto end = aArgs + aArgsCount;
	auto found = std::find( aArgs, end, aFlag );
	if( found != end && ++found != end )
	{
		std::stringstream ss;
		T result;
		ss << aArgs;
		ss >> result;
		return result;
	}
	return aDefaultValue;
}

}