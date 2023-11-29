#include "CTableauState.h"

#include <set>

#include "Constants.h"


namespace blacksmith
{

CTableauState::CSetState::CSetState() :
	mCount( 0 )
{
	mPieceSequence.reserve( CTableau::CHESS_PIECES().size() );
}

unsigned short CTableauState::CSetState::Update( const std::string& aPiece )
{
	// If it is a wildcard, just keep it going!
	if( aPiece == "W" )
		mPieceSequence.push_back( aPiece );
	// If it continues the sequence
	else if( !mChessSet || *mChessSet == CTableau::CHESS_PIECES().contains( aPiece ) )
	{
		const auto& found = std::find( mPieceSequence.begin(), mPieceSequence.end(), aPiece );
		// If repeated, delete that portion and reset count before adding new
		if( found != mPieceSequence.end() )
		{
			mPieceSequence.erase( mPieceSequence.begin(), found + 1 );
			mCount = 0;
		}
		mPieceSequence.push_back( aPiece );
		mChessSet = CTableau::CHESS_PIECES().contains( aPiece );

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
	if( mPieceSequence.size() == CTableau::CHESS_PIECES().size() )
	{
		++mCount;
		mPieceSequence.clear();
		mChessSet = !*mChessSet;
		return 20 * mCount;
	}
	else
		return 1;
}

void CTableauState::CSetState::Reset()
{
	*this = CSetState();
}

CTableauState::CMultiState::CMultiState() :
	mPiece( "E" ),
	mCount( 0 )
{
}

unsigned short CTableauState::CMultiState::Update( const std::string& aPiece )
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

void CTableauState::CMultiState::Reset()
{
	*this = CMultiState();
}

CTableauState::CTableauState( const CTableau& aTableau ) :
	mTableau( aTableau )
{
}

const CTableau& CTableauState::GetTableau() const
{
	return mTableau;
}

const unsigned short& CTableauState::GetScore() const
{
	return mScore;
}

CTableauState::destination CTableauState::Randomize( const destination& aOrigin ) const
{
	destinations dests; // Save all possible aDestinations
	dests.reserve( MAXPOS );
	MoveWildcard( aOrigin, dests );
	if( dests.size() > 0 ) // If it's possible
		return dests[ rand() % dests.size() ];
	else
		return { -1,-1 };
}

CTableauState::destination CTableauState::Move( const destination& aOrigin, std::mt19937_64& aRNG )
{
	destinations destinations; // Save all possible aDestinations
	if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "E" )		  // Empty
		return { -1,-1 };
	else if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "Q" )		  // Queen
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( aOrigin, destinations );
		MoveStraight( aOrigin, destinations );
	}
	else if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "B" ) // Bishop
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( aOrigin, destinations );
	}
	else if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "R" ) // Rook
	{
		destinations.reserve( MOVEPOS );
		MoveStraight( aOrigin, destinations );
	}
	else if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "K" ) // Knight
	{
		destinations.reserve( MOVEPOS_K );
		MoveKnight( aOrigin, destinations );
	}
	else if( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( MAXPOS );
		MoveWildcard( aOrigin, destinations );
	}
	else
	{
		destinations.reserve( MOVEPOS_K );
		MoveDiagonal( aOrigin, destinations, std::stoi( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] ) );
		MoveStraight( aOrigin, destinations, std::stoi( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] ) );
	}
	mScore += Update( mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] );
	mTableau.Pieces()[ aOrigin.first ][ aOrigin.second ] = "E";
	if( destinations.size() > 0 ) // If it's possible
		return destinations[ aRNG() % destinations.size() ];
	else
		return { -1,-1 };
}

void CTableauState::MoveDiagonal( const destination& aOrigin, destinations& aDestinations ) const
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

void CTableauState::MoveDiagonal( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second - aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second + aSteps } );
}

void CTableauState::MoveStraight( const destination& aOrigin, destinations& aDestinations ) const
{
	AppendDestination( aDestinations, aOrigin, { 0, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { 5, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 0 } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, 5 } );
}

void CTableauState::MoveStraight( const destination& aOrigin, destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, aOrigin, { aOrigin.first + aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first - aSteps, aOrigin.second } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second + aSteps } );
	AppendDestination( aDestinations, aOrigin, { aOrigin.first, aOrigin.second - aSteps } );
}

void CTableauState::MoveKnight( const destination& aOrigin, destinations& aDestinations ) const
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

void CTableauState::MoveWildcard( const destination& aOrigin, destinations& aDestinations ) const
{
	for( int i = 0; i < ROW; i++ )
		for( int j = 0; j < COL; j++ )
			AppendDestination( aDestinations, aOrigin, { i, j } );
	return;
}

void CTableauState::AppendDestination( destinations& aDestinations, const destination& aOrigin, const destination& aDestination ) const
{
	if( aOrigin != aDestination && mTableau.IsInside( aDestination ) && mTableau.GetPieces()[ aDestination.first ][ aDestination.second ] != "E" )
		aDestinations.push_back( aDestination );
}

unsigned short CTableauState::Update( const std::string& aPiece )
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

}