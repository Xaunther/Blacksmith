#include "CInputArguments.h"

#include <iostream>
#include <random>
#include <sstream>

namespace
{

template <typename T> struct is_optional : std::false_type {};
template <typename T> struct is_optional<std::optional<T>> : std::true_type {};

std::optional<CInputArguments::coordinates> CreateInitialCoordinates( const int& aArgsCount, const char** aArgs );

template <typename T> std::enable_if_t<is_optional<T>::value, T>
GetArgument( const int& aArgsCount, const char** aArgs, const std::string_view aFlag );

template <typename T> std::enable_if_t<!is_optional<T>::value, T>
GetArgument( const int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue = T{} );

void ShowHelp();

}

CInputArguments::CInputArguments( const int& aArgsCount, const char** aArgs ) :
	mMaxSteps( GetArgument<unsigned long>( aArgsCount, aArgs, "-n", 10000000 ) ),
	mSeed( GetArgument<unsigned long>( aArgsCount, aArgs, "-s", std::random_device()( ) ) ),
	mBoardFileName( GetArgument<std::string>( aArgsCount, aArgs, "-i", "board.dat" ) ),
	mBestPatternFileName( GetArgument<std::string>( aArgsCount, aArgs, "-o", "Best-pattern.txt" ) ),
	mInitialCoordinates( CreateInitialCoordinates( aArgsCount, aArgs ) ),
	mSpeed( GetArgument<bool>( aArgsCount, aArgs, "--speed", false ) )
{
	const auto end = aArgs + aArgsCount;
	if( std::find( aArgs, end, std::string_view{ "-h" } ) != end )
	{
		ShowHelp();
		mRun = false;
	}
}

namespace
{

std::optional<CInputArguments::coordinates> CreateInitialCoordinates( const int& aArgsCount, const char** aArgs )
{
	const auto& row = GetArgument<std::optional<CInputArguments::coordinates::first_type>>( aArgsCount, aArgs, "--row" );
	const auto& col = GetArgument<std::optional<CInputArguments::coordinates::second_type>>( aArgsCount, aArgs, "--col" );
	return row && col ? std::make_optional( std::make_pair( *row, *col ) ) : std::optional<CInputArguments::coordinates>{};
}
template <typename T> std::enable_if_t<is_optional<T>::value, T> GetArgument( const int& aArgsCount, const char** aArgs, const std::string_view aFlag )
{
	const auto end = aArgs + aArgsCount;
	return ( std::find( aArgs, end, aFlag ) != end ) ? GetArgument<typename T::value_type>( aArgsCount, aArgs, aFlag ) : T();
}

template <typename T> std::enable_if_t<!is_optional<T>::value, T> GetArgument( const int& aArgsCount, const char** aArgs, const std::string_view aFlag, const T& aDefaultValue )
{
	const auto end = aArgs + aArgsCount;
	auto found = std::find( aArgs, end, aFlag );
	if( found != end && ++found != end )
	{
		std::stringstream ss;
		T result;
		ss << *found;
		ss >> result;
		return result;
	}
	return aDefaultValue;
}

void ShowHelp()
{
	std::cout << "Options\n";
	std::cout << "  -n <max-number-of-tries> = Specifies maximum number of games to simulate (default: 10000000).\n";
	std::cout << "  -s <random-seed>         = Specifies a concrete random seed.\n";
	std::cout << "  -i <input-board-file>    = Sets the input board file (default: board.dat)\n";
	std::cout << "  -o <output-results-file> = Sets the output results file (default: Best-pattern.txt)\n";
	std::cout << "  --row <initial-row>      = Sets the initial row index (starting at 0) to hit (default: random).\n";
	std::cout << "  --col <initial-column>   = Sets the initial column index to hit (starting at 0) (default: random).\n";
	std::cout << "  --speed                  = If set, stops simulation when the first game that clears the whole board is found.\n";
	std::cout << "  -h                       = Shows this help.\n";
}

}