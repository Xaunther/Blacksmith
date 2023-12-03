#include "CTableauState.h"

#include <set>

namespace blacksmith
{

CTableauState::CSetState::CSetState() :
	mCount( 0 )
{
}

unsigned short CTableauState::CSetState::Update( const std::string& aPiece )
{
	if( aPiece != "W" )
	{
		const auto pieceSet = CTableau::CHESS_PIECES().contains( aPiece );
		if( !mChessSet )
			mChessSet = pieceSet;
		// If it continues the sequence
		else if( *mChessSet == pieceSet )
		{
			auto found = std::find( mPieceSequence.cbegin(), mPieceSequence.cend(), aPiece );
			// If repeated, delete that portion and reset count before adding new
			if( found != mPieceSequence.cend() )
			{
				mPieceSequence.erase( mPieceSequence.cbegin(), ++found );
				mCount = 0;
			}
		}
		// Otherwise, reset
		else
		{
			mPieceSequence.clear();
			mChessSet = !*mChessSet;
			mCount = 0;
		}
	}
	mPieceSequence.push_back( aPiece );

	// Detect score given by this piece
	if( mChessSet && mPieceSequence.size() >= CTableau::CHESS_PIECES().size() )
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

CTableauState::CTableauState( const CTableau& aTableau, const std::optional<coordinates>& aCurrentPosition ) :
	mTableau( aTableau ),
	mCurrentPosition( aCurrentPosition )
{
}

const CTableau& CTableauState::GetTableau() const
{
	return mTableau;
}

const std::optional<CTableauState::coordinates>& CTableauState::GetCurrentPosition() const
{
	return mCurrentPosition;
}

const unsigned short& CTableauState::GetScore() const
{
	return mScore;
}

const CTableauState::coordinates& CTableauState::SetCurrentPositionAtRandom( std::mt19937_64& aRNG )
{
	mCurrentPosition = mTableau.GetRandomPieceCoordinates( aRNG );
	return *mCurrentPosition;
}

std::optional<CTableauState::coordinates> CTableauState::Move( std::mt19937_64& aRNG )
{
	if( !mCurrentPosition ) return {};

	mScore += Update( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) );
	return mCurrentPosition = mTableau.HitPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second, aRNG );
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