#include "CTableauState.h"

#include <set>

#include "Constants.h"

namespace blacksmith
{

constexpr unsigned short CTableauState::NUMERIC_DESTINATIONS_COUNT()
{
	return 8;
}

constexpr unsigned short CTableauState::KQ_DESTINATIONS_COUNT()
{
	return 8;
}

constexpr unsigned short CTableauState::RB_DESTINATIONS_COUNT()
{
	return 4;
}

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
	coordinates_vector dests; // Save all possible aDestinations
	dests.reserve( mTableau.Size() );
	MoveWildcard( dests );
	if( dests.size() > 0 ) // If it's possible
		mCurrentPosition = dests[ aRNG() % dests.size() ];

	return *mCurrentPosition;
}

std::optional<CTableauState::coordinates> CTableauState::Move( std::mt19937_64& aRNG )
{
	coordinates_vector destinations; // Save all possible aDestinations
	if( !mCurrentPosition || mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "E" )		  // Empty
		return {};
	else if( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "Q" )		  // Queen
	{
		destinations.reserve( KQ_DESTINATIONS_COUNT() );
		MoveDiagonal( destinations );
		MoveStraight( destinations );
	}
	else if( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "B" ) // Bishop
	{
		destinations.reserve( RB_DESTINATIONS_COUNT() );
		MoveDiagonal( destinations );
	}
	else if( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "R" ) // Rook
	{
		destinations.reserve( RB_DESTINATIONS_COUNT() );
		MoveStraight( destinations );
	}
	else if( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "K" ) // Knight
	{
		destinations.reserve( KQ_DESTINATIONS_COUNT() );
		MoveKnight( destinations );
	}
	else if( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == "W" ) // Rum (WILDCARD!)
	{
		destinations.reserve( mTableau.Size() );
		MoveWildcard( destinations );
	}
	else
	{
		destinations.reserve( NUMERIC_DESTINATIONS_COUNT() );
		MoveDiagonal( destinations, std::stoi( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) ) );
		MoveStraight( destinations, std::stoi( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) ) );
	}
	mScore += Update( mTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) );
	mTableau.SetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second, "E" );
	if( !destinations.empty() ) // If it's possible
		mCurrentPosition = destinations[ aRNG() % destinations.size() ];
	else
		mCurrentPosition.reset();

	return mCurrentPosition;
}

void CTableauState::MoveDiagonal( coordinates_vector& aDestinations ) const
{
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - std::min( ( *mCurrentPosition ).first - 0, ( *mCurrentPosition ).second - 0 ),
		( *mCurrentPosition ).second - std::min( ( *mCurrentPosition ).first - 0, ( *mCurrentPosition ).second - 0 ) } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + std::min( 5 - ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - 0 ),
		( *mCurrentPosition ).second - std::min( 5 - ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - 0 ) } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + std::min( 5 - ( *mCurrentPosition ).first, 5 - ( *mCurrentPosition ).second ),
		( *mCurrentPosition ).second + std::min( 5 - ( *mCurrentPosition ).first, 5 - ( *mCurrentPosition ).second ) } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - std::min( ( *mCurrentPosition ).first - 0, 5 - ( *mCurrentPosition ).second ),
		( *mCurrentPosition ).second + std::min( ( *mCurrentPosition ).first - 0, 5 - ( *mCurrentPosition ).second ) } );
}

void CTableauState::MoveDiagonal( coordinates_vector& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second - aSteps } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second - aSteps } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second + aSteps } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second + aSteps } );
}

void CTableauState::MoveStraight( coordinates_vector& aDestinations ) const
{
	AppendDestination( aDestinations, { 0, ( *mCurrentPosition ).second } );
	AppendDestination( aDestinations, { 5, ( *mCurrentPosition ).second } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first, 0 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first, 5 } );
}

void CTableauState::MoveStraight( coordinates_vector& aDestinations, int aSteps ) const
{
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first, ( *mCurrentPosition ).second + aSteps } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - aSteps } );
}

void CTableauState::MoveKnight( coordinates_vector& aDestinations ) const
{
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - 1, ( *mCurrentPosition ).second - 2 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - 1, ( *mCurrentPosition ).second + 2 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + 1, ( *mCurrentPosition ).second - 2 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + 1, ( *mCurrentPosition ).second + 2 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - 2, ( *mCurrentPosition ).second - 1 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first - 2, ( *mCurrentPosition ).second + 1 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + 2, ( *mCurrentPosition ).second - 1 } );
	AppendDestination( aDestinations, { ( *mCurrentPosition ).first + 2, ( *mCurrentPosition ).second + 1 } );
}

void CTableauState::MoveWildcard( coordinates_vector& aDestinations ) const
{
	for( int i = 0; i < mTableau.GetRows(); i++ )
		for( int j = 0; j < mTableau.GetRows(); j++ )
			AppendDestination( aDestinations, { i, j } );
	return;
}

void CTableauState::AppendDestination( coordinates_vector& aDestinations, const coordinates& aDestination ) const
{
	if( *mCurrentPosition != aDestination && mTableau.IsInside( aDestination.first, aDestination.second ) && mTableau.GetPiece( aDestination.first, aDestination.second ) != "E" )
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