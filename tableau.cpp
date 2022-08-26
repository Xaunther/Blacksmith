// Implementation of tableau

#include "tableau.h"
#include <fstream>
#include <string>
#include <set>

namespace blacksmith
{

namespace
{

static const std::set<std::string> PIECES = { "Q","B","R","K","W","E","1","2","3","4" };

const tableau::piece_matrix& Check( const tableau::piece_matrix& aPieces );

}

// Initializer
tableau::tableau()
{
	for( auto& _row : mPieces )
		for( auto& piece : _row )
			piece = "E";
}

tableau::tableau( const std::string& aFileName )
{
	Load( aFileName );
	Check( mPieces );
}

// Load "map" from file, file must follow the codes
void tableau::Load( const std::string& aFileName )
{
	std::ifstream infile;
	std::string basura;
	infile.open( aFileName.c_str() );
	for( auto& _row : mPieces )
		for( auto& piece : _row )
			infile >> piece;
	infile.close();
}

// Move from current place to another suitable one, randomly. Return next point
tableau::destination tableau::Move( const destination& aOrigin )
{
	destinations destinations; // Save all possible aDestinations
	if( mPieces[ aOrigin.first ][ aOrigin.second ] == "E" )		  // Empty
		return { -1,-1 };
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "Q" )		  // Queen
	{
		destinations.reserve( movepos );
		MoveDiagonal( aOrigin, destinations );
		MoveStraight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "B" ) // Bishop
	{
		destinations.reserve( movepos );
		MoveDiagonal( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "R" ) // Rook
	{
		destinations.reserve( movepos );
		MoveStraight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "K" ) // Knight
	{
		destinations.reserve( movepos_K );
		MoveKnight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( maxpos );
		MoveWildcard( aOrigin, destinations );
	}
	else
	{
		destinations.reserve( movepos_K );
		MoveDiagonal( aOrigin, destinations, std::stoi( mPieces[ aOrigin.first ][ aOrigin.second ] ) );
		MoveStraight( aOrigin, destinations, std::stoi( mPieces[ aOrigin.first ][ aOrigin.second ] ) );
	}
	mPieces[ aOrigin.first ][ aOrigin.second ] = "E";
	if( destinations.size() > 0 ) // If it's possible
		return destinations[ rand() % destinations.size() ];
	else
		return { -1,-1 };
}

void tableau::MoveDiagonal( const destination& aOrigin, destinations& aDestinations ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - std::min( aOrigin.first - 0, aOrigin.second - 0 ),
		aOrigin.second - std::min( aOrigin.first - 0, aOrigin.second - 0 ) } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + std::min( 5 - aOrigin.first, aOrigin.second - 0 ),
		aOrigin.second - std::min( 5 - aOrigin.first, aOrigin.second - 0 ) } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + std::min( 5 - aOrigin.first, 5 - aOrigin.second ),
		aOrigin.second + std::min( 5 - aOrigin.first, 5 - aOrigin.second ) } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - std::min( aOrigin.first - 0, 5 - aOrigin.second ),
		aOrigin.second + std::min( aOrigin.first - 0, 5 - aOrigin.second ) } );
}

void tableau::MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second + aSteps } );
}

void tableau::MoveStraight( const destination& aOrigin, destinations& aDestinations ) const
{
	AppendDestination( aDestinations, aOrigin, { 0, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { 5, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 0 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 5 } );
}

void tableau::MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second - aSteps } );
}

void tableau::MoveKnight( const destination& aOrigin, destinations& aDestinations ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - 1, aOrigin.second - 2 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - 1, aOrigin.second + 2 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + 1, aOrigin.second - 2 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + 1, aOrigin.second + 2 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - 2, aOrigin.second - 1 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - 2, aOrigin.second + 1 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + 2, aOrigin.second - 1 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + 2, aOrigin.second + 1 } );
}

void tableau::MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const
{
	for( int i = 0; i < row; i++ )
		for( int j = 0; j < col; j++ )
			AppendDestination( aDestinations, aOrigin, { i, j } );
	return;
}

tableau::destination tableau::Randomize( const destination& aOrigin ) const
{
	destinations dests; // Save all possible aDestinations
	dests.reserve( maxpos );
	MoveWildcard( aOrigin, dests );
	if( dests.size() > 0 ) // If it's possible
		return dests[ rand() % dests.size() ];
	else
		return { -1,-1 };
}

const tableau::piece_matrix& tableau::GetPieces() const
{
	return mPieces;
}

const std::string& tableau::GetPiece( const destination& aDestination ) const
{
	return mPieces.at( aDestination.first ).at( aDestination.second );
}

void tableau::SetPiece( const destination& aOrigin, const std::string& aPiece )
{
	if( IsInside( aOrigin ) )
		mPieces[ aOrigin.first ][ aOrigin.second ] = aPiece;
}

bool tableau::IsInside( const destination& aDestination ) const
{
	return aDestination.first >= 0 && aDestination.first < row&& aDestination.second >= 0 && aDestination.second < col;
}

int tableau::CountPieces() const
{
	// Loop around and count non-empty slots
	int count = 0;
	for( const auto& _row : mPieces )
		for( const auto& piece : _row )
			if( piece != "E" )
				count++;
	return count;
}

void tableau::AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const
{
	if( aOrigin != aDestination && IsInside( aDestination ) && mPieces[ aDestination.first ][ aDestination.second ] != "E" )
		aDestinations.push_back( aDestination );
}

namespace
{

const tableau::piece_matrix& Check( const tableau::piece_matrix& aPieces )
{
	for( const auto& _row : aPieces )
		for( const auto& piece : _row )
			if( PIECES.find( piece ) == PIECES.cend() )
				throw std::runtime_error( "No existe la pieza '" + piece + "'." );
	return aPieces;
}

}

};