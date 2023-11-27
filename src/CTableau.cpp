#include "CTableau.h"
#include <fstream>
#include <string>
#include <set>

namespace blacksmith
{

namespace
{

static const std::set<std::string> PIECES = { "Q","B","R","K","W","E","1","2","3","4" };
static const std::set<std::string> CHESS_PIECES = { "Q","B","R","K" };
static const std::set<std::string> NUMBER_PIECES = { "1","2","3","4" };

const CTableau::piece_matrix& Check( const CTableau::piece_matrix& aPieces );

}

CTableau::CComboState::CSetState::CSetState() :
	mCount( 0 )
{
	mPieceSequence.reserve( CHESS_PIECES.size() );
}

unsigned short CTableau::CComboState::CSetState::Update( const std::string& aPiece )
{
	// If it is a wildcard, just keep it going!
	if( aPiece == "W" )
		mPieceSequence.push_back( aPiece );
	// If it continues the sequence
	else if( !mChessSet || *mChessSet == CHESS_PIECES.contains( aPiece ) )
	{
		const auto& found = std::find( mPieceSequence.begin(), mPieceSequence.end(), aPiece );
		// If repeated, delete that portion and reset count before adding new
		if( found != mPieceSequence.end() )
		{
			mPieceSequence.erase( mPieceSequence.begin(), found + 1 );
			mCount = 0;
		}
		mPieceSequence.push_back( aPiece );
		mChessSet = CHESS_PIECES.contains( aPiece );

	}
	// Otherwise, reset
	else
	{
		mPieceSequence.clear();
		mPieceSequence.push_back( aPiece );
		mChessSet = !*mChessSet;
		mCount = 0;
	}

	// Detect score given by this piece
	if( mPieceSequence.size() == CHESS_PIECES.size() )
	{
		++mCount;
		mPieceSequence.clear();
		mChessSet = !*mChessSet;
		return 20 * mCount;
	}
	else
		return 1;
}

void CTableau::CComboState::CSetState::Reset()
{
	*this = CSetState();
}

CTableau::CComboState::CMultiState::CMultiState() :
	mPiece( "E" ),
	mCount( 0 )
{
}

unsigned short CTableau::CComboState::CMultiState::Update( const std::string& aPiece )
{
	if( mPiece == aPiece )
		++mCount;
	else
	{
		mPiece = aPiece;
		mCount = 1;
	}
	return mCount;
}

void CTableau::CComboState::CMultiState::Reset()
{
	*this = CMultiState();
}

unsigned short CTableau::CComboState::Update( const std::string& aPiece )
{
	const auto& setScore = mSetState.Update( aPiece );
	if( setScore > 1 )
	{
		mMultiState.Reset();
		return setScore;
	}
	const auto& multiScore = mMultiState.Update( aPiece );
	if( multiScore > 1 )
	{
		mSetState.Reset();
		return multiScore;
	}
	return 1;
}

// Initializer
CTableau::CTableau() :
	mScore( 0 )
{
	for( auto& _row : mPieces )
		for( auto& piece : _row )
			piece = "E";
}

CTableau::CTableau( const std::string& aFileName ) :
	mScore( 0 )
{
	Load( aFileName );
	Check( mPieces );
}

// Load "map" from file, file must follow the codes
void CTableau::Load( const std::string& aFileName )
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
CTableau::destination CTableau::Move( const destination& aOrigin, std::mt19937_64& aRNG )
{
	destinations destinations; // Save all possible aDestinations
	if( mPieces[ aOrigin.first ][ aOrigin.second ] == "E" )		  // Empty
		return { -1,-1 };
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "Q" )		  // Queen
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( aOrigin, destinations );
		MoveStraight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "B" ) // Bishop
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "R" ) // Rook
	{
		destinations.reserve( MOVEPOS );
		MoveStraight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "K" ) // Knight
	{
		destinations.reserve( MOVEPOS_K );
		MoveKnight( aOrigin, destinations );
	}
	else if( mPieces[ aOrigin.first ][ aOrigin.second ] == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( MAXPOS );
		MoveWildcard( aOrigin, destinations );
	}
	else
	{
		destinations.reserve( MOVEPOS_K );
		MoveDiagonal( aOrigin, destinations, std::stoi( mPieces[ aOrigin.first ][ aOrigin.second ] ) );
		MoveStraight( aOrigin, destinations, std::stoi( mPieces[ aOrigin.first ][ aOrigin.second ] ) );
	}
	mScore += mComboState.Update( mPieces[ aOrigin.first ][ aOrigin.second ] );
	mPieces[ aOrigin.first ][ aOrigin.second ] = "E";
	if( destinations.size() > 0 ) // If it's possible
		return destinations[ aRNG() % destinations.size() ];
	else
		return { -1,-1 };
}

void CTableau::MoveDiagonal( const destination& aOrigin, destinations& aDestinations ) const
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

void CTableau::MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second + aSteps } );
}

void CTableau::MoveStraight( const destination& aOrigin, destinations& aDestinations ) const
{
	AppendDestination( aDestinations, aOrigin, { 0, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { 5, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 0 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 5 } );
}

void CTableau::MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second - aSteps } );
}

void CTableau::MoveKnight( const destination& aOrigin, destinations& aDestinations ) const
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

void CTableau::MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const
{
	for( int i = 0; i < ROW; i++ )
		for( int j = 0; j < COL; j++ )
			AppendDestination( aDestinations, aOrigin, { i, j } );
	return;
}

CTableau::destination CTableau::Randomize( const destination& aOrigin ) const
{
	destinations dests; // Save all possible aDestinations
	dests.reserve( MAXPOS );
	MoveWildcard( aOrigin, dests );
	if( dests.size() > 0 ) // If it's possible
		return dests[ rand() % dests.size() ];
	else
		return { -1,-1 };
}

const CTableau::piece_matrix& CTableau::GetPieces() const
{
	return mPieces;
}

const std::string& CTableau::GetPiece( const destination& aDestination ) const
{
	return mPieces.at( aDestination.first ).at( aDestination.second );
}

const unsigned short& CTableau::GetScore() const
{
	return mScore;
}

void CTableau::SetPiece( const destination& aOrigin, const std::string& aPiece )
{
	if( IsInside( aOrigin ) )
		mPieces[ aOrigin.first ][ aOrigin.second ] = aPiece;
}

bool CTableau::IsInside( const destination& aDestination ) const
{
	return aDestination.first >= 0 && aDestination.first < ROW && aDestination.second >= 0 && aDestination.second < COL;
}

unsigned int CTableau::CountPieces() const
{
	// Loop around and count non-empty slots
	int count = 0;
	for( const auto& _row : mPieces )
		for( const auto& piece : _row )
			if( piece != "E" )
				count++;
	return count;
}

void CTableau::AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const
{
	if( aOrigin != aDestination && IsInside( aDestination ) && mPieces[ aDestination.first ][ aDestination.second ] != "E" )
		aDestinations.push_back( aDestination );
}

namespace
{

const CTableau::piece_matrix& Check( const CTableau::piece_matrix& aPieces )
{
	for( const auto& _row : aPieces )
		for( const auto& piece : _row )
			if( PIECES.find( piece ) == PIECES.cend() )
				throw std::runtime_error( "No existe la pieza '" + piece + "'." );
	return aPieces;
}

}

};