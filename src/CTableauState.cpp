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

CTableauState::CTableauState( const CTableau& aTableau, const destination& aCurrentPosition ) :
	mTableau( aTableau ),
	mCurrentPosition( aCurrentPosition )
{
}

const CTableau& CTableauState::GetTableau() const
{
	return mTableau;
}

const CTableauState::destination& CTableauState::GetCurrentPosition() const
{
	return mCurrentPosition;
}

const unsigned short& CTableauState::GetScore() const
{
	return mScore;
}

CTableauState::destination CTableauState::Randomize()
{
	destinations dests; // Save all possible aDestinations
	dests.reserve( MAXPOS );
	MoveWildcard( dests );
	if( dests.size() > 0 ) // If it's possible
		mCurrentPosition = dests[ rand() % dests.size() ];
	else
		mCurrentPosition = { -1, -1 };

	return mCurrentPosition;
}

CTableauState::destination CTableauState::Move( std::mt19937_64& aRNG )
{
	destinations destinations; // Save all possible aDestinations
	if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "E" )		  // Empty
		return { -1,-1 };
	else if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "Q" )		  // Queen
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( destinations );
		MoveStraight( destinations );
	}
	else if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "B" ) // Bishop
	{
		destinations.reserve( MOVEPOS );
		MoveDiagonal( destinations );
	}
	else if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "R" ) // Rook
	{
		destinations.reserve( MOVEPOS );
		MoveStraight( destinations );
	}
	else if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "K" ) // Knight
	{
		destinations.reserve( MOVEPOS_K );
		MoveKnight( destinations );
	}
	else if( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( MAXPOS );
		MoveWildcard( destinations );
	}
	else
	{
		destinations.reserve( MOVEPOS_K );
		MoveDiagonal( destinations, std::stoi( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] ) );
		MoveStraight( destinations, std::stoi( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] ) );
	}
	mScore += Update( mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] );
	mTableau.Pieces()[ mCurrentPosition.first ][ mCurrentPosition.second ] = "E";
	if( destinations.size() > 0 ) // If it's possible
		mCurrentPosition = destinations[ aRNG() % destinations.size() ];
	else
		mCurrentPosition = { -1,-1 };

	return mCurrentPosition;
}

void CTableauState::MoveDiagonal( destinations& aDestinations ) const
{
	AppendDestination( aDestinations, { mCurrentPosition.first - std::min( mCurrentPosition.first - 0, mCurrentPosition.second - 0 ),
		mCurrentPosition.second - std::min( mCurrentPosition.first - 0, mCurrentPosition.second - 0 ) } );
	AppendDestination( aDestinations, { mCurrentPosition.first + std::min( 5 - mCurrentPosition.first, mCurrentPosition.second - 0 ),
		mCurrentPosition.second - std::min( 5 - mCurrentPosition.first, mCurrentPosition.second - 0 ) } );
	AppendDestination( aDestinations, { mCurrentPosition.first + std::min( 5 - mCurrentPosition.first, 5 - mCurrentPosition.second ),
		mCurrentPosition.second + std::min( 5 - mCurrentPosition.first, 5 - mCurrentPosition.second ) } );
	AppendDestination( aDestinations, { mCurrentPosition.first - std::min( mCurrentPosition.first - 0, 5 - mCurrentPosition.second ),
		mCurrentPosition.second + std::min( mCurrentPosition.first - 0, 5 - mCurrentPosition.second ) } );
}

void CTableauState::MoveDiagonal( destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, { mCurrentPosition.first - aSteps, mCurrentPosition.second - aSteps } );
	AppendDestination( aDestinations, { mCurrentPosition.first + aSteps, mCurrentPosition.second - aSteps } );
	AppendDestination( aDestinations, { mCurrentPosition.first + aSteps, mCurrentPosition.second + aSteps } );
	AppendDestination( aDestinations, { mCurrentPosition.first - aSteps, mCurrentPosition.second + aSteps } );
}

void CTableauState::MoveStraight( destinations& aDestinations ) const
{
	AppendDestination( aDestinations, { 0, mCurrentPosition.second } );
	AppendDestination( aDestinations, { 5, mCurrentPosition.second } );
	AppendDestination( aDestinations, { mCurrentPosition.first, 0 } );
	AppendDestination( aDestinations, { mCurrentPosition.first, 5 } );
}

void CTableauState::MoveStraight( destinations& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, { mCurrentPosition.first + aSteps, mCurrentPosition.second } );
	AppendDestination( aDestinations, { mCurrentPosition.first - aSteps, mCurrentPosition.second } );
	AppendDestination( aDestinations, { mCurrentPosition.first, mCurrentPosition.second + aSteps } );
	AppendDestination( aDestinations, { mCurrentPosition.first, mCurrentPosition.second - aSteps } );
}

void CTableauState::MoveKnight( destinations& aDestinations ) const
{
	AppendDestination( aDestinations, { mCurrentPosition.first - 1, mCurrentPosition.second - 2 } );
	AppendDestination( aDestinations, { mCurrentPosition.first - 1, mCurrentPosition.second + 2 } );
	AppendDestination( aDestinations, { mCurrentPosition.first + 1, mCurrentPosition.second - 2 } );
	AppendDestination( aDestinations, { mCurrentPosition.first + 1, mCurrentPosition.second + 2 } );
	AppendDestination( aDestinations, { mCurrentPosition.first - 2, mCurrentPosition.second - 1 } );
	AppendDestination( aDestinations, { mCurrentPosition.first - 2, mCurrentPosition.second + 1 } );
	AppendDestination( aDestinations, { mCurrentPosition.first + 2, mCurrentPosition.second - 1 } );
	AppendDestination( aDestinations, { mCurrentPosition.first + 2, mCurrentPosition.second + 1 } );
}

void CTableauState::MoveWildcard( destinations& aDestinations ) const
{
	for( int i = 0; i < ROW; i++ )
		for( int j = 0; j < COL; j++ )
			AppendDestination( aDestinations, { i, j } );
	return;
}

void CTableauState::AppendDestination( destinations& aDestinations, const destination& aDestination ) const
{
	if( mCurrentPosition != aDestination && mTableau.IsInside( aDestination ) && mTableau.GetPieces()[ aDestination.first ][ aDestination.second ] != "E" )
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