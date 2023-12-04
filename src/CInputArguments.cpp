#include "CInputArguments.h"

#include <random>
#include <sstream>

namespace
{

template <typename T> struct is_optional : std::false_type {};
template <typename T> struct is_optional<std::optional<T>> : std::true_type {};

std::optional<CInputArguments::coordinates> CreateInitialCoordinates( const unsigned int& aArgsCount, const char** aArgs );

template <typename T> std::enable_if_t<is_optional<T>::value, T>
GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag );

template <typename T> std::enable_if_t<!is_optional<T>::value, T>
GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue = T{} );

}

CInputArguments::CInputArguments( const unsigned int& aArgsCount, const char** aArgs ) :
	mMaxSteps( GetArgument<unsigned long>( aArgsCount, aArgs, "-n", 10000000 ) ),
	mSeed( GetArgument<unsigned long>( aArgsCount, aArgs, "-s", std::random_device()( ) ) ),
	mBoardFileName( GetArgument<std::string>( aArgsCount, aArgs, "-i", "board.dat" ) ),
	mBestPatternFileName( GetArgument<std::string>( aArgsCount, aArgs, "-o", "Best-pattern.txt" ) ),
	mInitialCoordinates( CreateInitialCoordinates( aArgsCount, aArgs ) )
{
}

namespace
{

std::optional<CInputArguments::coordinates> CreateInitialCoordinates( const unsigned int& aArgsCount, const char** aArgs )
{
	const auto& row = GetArgument<std::optional<CInputArguments::coordinates::first_type>>( aArgsCount, aArgs, "--row" );
	const auto& col = GetArgument<std::optional<CInputArguments::coordinates::second_type>>( aArgsCount, aArgs, "--col" );
	if( row && col ) return std::make_pair( *row, *col );
	else return {};
}
template <typename T> std::enable_if_t<is_optional<T>::value, T> GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag )
{
	return ( std::find( aArgs, aArgs + aArgsCount, aFlag ) != aArgs + aArgsCount ) ? GetArgument<typename T::value_type>( aArgsCount, aArgs, aFlag ) : T();
}

template <typename T> std::enable_if_t<!is_optional<T>::value, T> GetArgument( const unsigned int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue )
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