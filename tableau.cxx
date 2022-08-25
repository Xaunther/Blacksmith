// Implementation of tableau

#include "tableau.h"
#include <algorithm>
#include <fstream>
#include <stdlib.h>
#include <string>

// Initializer
tableau::tableau()
{
	for ( int i = 0; i < row; i++ )
	{
		for ( int j = 0; j < col; j++ )
		{
			mPieces[ i ][ j ] = "E";
		}
	}
}

tableau::tableau( const std::string& aFileName )
{
	Load( aFileName );
}

// Load "map" from file, file must follow the codes
void tableau::Load( const std::string& aFileName )
{
	std::ifstream infile;
	std::string basura;
	infile.open( aFileName.c_str() );
	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			infile >> mPieces[ i ][ j ];
	infile.close();
}

// Move from current place to another suitable one, randomly. Return next point
tableau::destination tableau::Move( const destination& aOrigin )
{
	destinations destinations; // Save all possible aDestinations
	if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "Q" )		  // Queen
	{
		destinations.reserve( movepos );
		MoveDiagonal( aOrigin, destinations );
		MoveStraight( aOrigin, destinations );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "B" ) // Bishop
	{
		destinations.reserve( movepos );
		MoveDiagonal( aOrigin, destinations );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "R" ) // Rook
	{
		destinations.reserve( movepos );
		MoveStraight( aOrigin, destinations );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "K" ) // Knight
	{
		destinations.reserve( movepos_K );
		MoveKnight( aOrigin, destinations );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "1" ) // 1
	{
		destinations.reserve( movepos_K );
		MoveDiagonal( aOrigin, destinations, 1 );
		MoveStraight( aOrigin, destinations, 1 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "2" ) // 2
	{
		destinations.reserve( movepos_K );
		MoveDiagonal( aOrigin, destinations, 2 );
		MoveStraight( aOrigin, destinations, 2 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "3" ) // 3
	{
		destinations.reserve( movepos_K );
		MoveDiagonal( aOrigin, destinations, 3 );
		MoveStraight( aOrigin, destinations, 3 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "4" ) // 4
	{
		destinations.reserve( movepos_K );
		MoveDiagonal( aOrigin, destinations, 4 );
		MoveStraight( aOrigin, destinations, 4 );
	}
	else if ( mPieces[ aOrigin.first ][ aOrigin.second ] == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( maxpos );
		MoveWildcard( aOrigin, destinations );
	}
	mPieces[ aOrigin.first ][ aOrigin.second ] = "E"; // Remove the piece, and make it emptydestdests
	if ( destinations.size() > 0 ) // If it's possible
		return destinations[ rand() % destinations.size() ];
	else
		return { -1,-1 };
}

void tableau::MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	// Junk destination variable to test before adding
	destinations dests;
	dests.reserve( movepos );
	if ( aSteps < 0 )
	{
		dests.emplace_back( aOrigin.first - std::min( aOrigin.first - 0, aOrigin.second - 0 ),
			aOrigin.second - std::min( aOrigin.first - 0, aOrigin.second - 0 ) );
		dests.emplace_back( aOrigin.first + std::min( 5 - aOrigin.first, aOrigin.second - 0 ),
			aOrigin.second - std::min( 5 - aOrigin.first, aOrigin.second - 0 ) );
		dests.emplace_back( aOrigin.first + std::min( 5 - aOrigin.first, 5 - aOrigin.second ),
			aOrigin.second + std::min( 5 - aOrigin.first, 5 - aOrigin.second ) );
		dests.emplace_back( aOrigin.first - std::min( aOrigin.first - 0, 5 - aOrigin.second ),
			aOrigin.second + std::min( aOrigin.first - 0, 5 - aOrigin.second ) );
	}
	else
	{
		dests.emplace_back( aOrigin.first - aSteps, aOrigin.second - aSteps );
		dests.emplace_back( aOrigin.first + aSteps, aOrigin.second - aSteps );
		dests.emplace_back( aOrigin.first + aSteps, aOrigin.second + aSteps );
		dests.emplace_back( aOrigin.first - aSteps, aOrigin.second + aSteps );
		for ( auto destIt = dests.begin(); destIt != dests.end(); )
		{
			const auto& dest = *destIt;
			// If off-limits, erase
			if ( dest.first < 0 || dest.first > 5 || dest.second < 0 || dest.second > 5 || mPieces[ dest.first ][ dest.second ] == "E" )
				destIt = dests.erase( destIt );
			else
				++destIt;
		}
	}
	aDestinations.insert( aDestinations.end(), std::make_move_iterator( dests.begin() ), std::make_move_iterator( dests.end() ) );
	return;
}

void tableau::MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	// Junk destination variable to test before adding
	destinations dests;
	dests.reserve( movepos );
	if ( aSteps < 0 )
	{
		dests.emplace_back( 0, aOrigin.second );
		dests.emplace_back( 5, aOrigin.second );
		dests.emplace_back( aOrigin.first, 0 );
		dests.emplace_back( aOrigin.first, 5 );
	}
	else
	{
		dests.emplace_back( aOrigin.first + aSteps, aOrigin.second );
		dests.emplace_back( aOrigin.first - aSteps, aOrigin.second );
		dests.emplace_back( aOrigin.first, aOrigin.second + aSteps );
		dests.emplace_back( aOrigin.first, aOrigin.second - aSteps );
		for ( auto destIt = dests.begin(); destIt != dests.end(); )
		{
			const auto& dest = *destIt;
			// If off-limits, erase
			if ( dest.first < 0 || dest.first > 5 || dest.second < 0 || dest.second > 5 || mPieces[ dest.first ][ dest.second ] == "E" )
				destIt = dests.erase( destIt );
			else
				++destIt;
		}
	}
	aDestinations.insert( aDestinations.end(), std::make_move_iterator( dests.begin() ), std::make_move_iterator( dests.end() ) );
	return;
}

void tableau::MoveKnight( const destination& aOrigin, destinations& aDestinations ) const
{
	// Junk destination variable to test before adding
	destinations dests;
	dests.reserve( movepos_K );

	dests.emplace_back( aOrigin.first - 1, aOrigin.second - 2 );
	dests.emplace_back( aOrigin.first - 1, aOrigin.second + 2 );
	dests.emplace_back( aOrigin.first + 1, aOrigin.second - 2 );
	dests.emplace_back( aOrigin.first + 1, aOrigin.second + 2 );
	dests.emplace_back( aOrigin.first - 2, aOrigin.second - 1 );
	dests.emplace_back( aOrigin.first - 2, aOrigin.second + 1 );
	dests.emplace_back( aOrigin.first + 2, aOrigin.second - 1 );
	dests.emplace_back( aOrigin.first + 2, aOrigin.second + 1 );

	for ( auto destIt = dests.begin(); destIt != dests.end(); )
	{
		const auto& dest = *destIt;
		// If off-limits, erase
		if ( dest.first < 0 || dest.first > 5 || dest.second < 0 || dest.second > 5 || mPieces[ dest.first ][ dest.second ] == "E" )
			destIt = dests.erase( destIt );
		else
			++destIt;
	}
	aDestinations.insert( aDestinations.end(), std::make_move_iterator( dests.begin() ), std::make_move_iterator( dests.end() ) );
	return;
}

void tableau::MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const
{
	aDestinations.reserve( maxpos );
	// Wildcard, all are possible
	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			if ( aOrigin != destination{ i, j } && mPieces[ i ][ j ] != "E" )
				aDestinations.emplace_back( i, j );
	return;
}

tableau::destination tableau::Randomize( const destination& aOrigin ) const
{
	destinations dests; // Save all possible aDestinations
	dests.reserve( maxpos );
	MoveWildcard( aOrigin, dests );
	if ( dests.size() > 0 ) // If it's possible
		return dests[ rand() % dests.size() ];
	else
		return { -1,-1 };
}

const tableau::piece_matrix& tableau::GetPieces() const
{
	return mPieces;
}

void tableau::SetPiece( const destination& aOrigin, const std::string& aPiece )
{
	mPieces[ aOrigin.first ][ aOrigin.second ] = aPiece;
}

int tableau::CountPieces() const
{
	// Loop around and count non-empty slots
	int count = 0;
	for ( int i = 0; i < row; i++ )
		for ( int j = 0; j < col; j++ )
			if ( mPieces[ i ][ j ] != "E" )
				count++;
	return count;
}
