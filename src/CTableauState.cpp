#include "CTableauState.h"

#include <fstream>
#include <iomanip>
#include <set>

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
}

unsigned short CTableauState::CSetState::Update( const piece_type& aPiece )
{
	if( aPiece != CTableau::E_PIECE_TYPE::WILDCARD )
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
	mPiece( CTableau::E_PIECE_TYPE::EMPTY ),
	mCount( 0 )
{
}

unsigned short CTableauState::CMultiState::Update( const piece_type& aPiece )
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

CTableauState::CTableauState( const std::optional<coordinates>& aCurrentPosition ) :
	mCurrentPosition( aCurrentPosition )
{
}

const CTableauState::history& CTableauState::GetHistory() const
{
	return mHistory;
}

const std::optional<CTableauState::coordinates>& CTableauState::GetCurrentPosition() const
{
	return mCurrentPosition;
}

const unsigned short& CTableauState::GetScore() const
{
	return mScore;
}

const CTableauState::coordinates& CTableauState::SetCurrentPositionAtRandom( const CTableau& aTableau, std::mt19937_64& aRNG )
{
	coordinates_vector dests; // Save all possible aDestinations
	dests.reserve( aTableau.Size() );
	MoveWildcard( aTableau, dests );
	if( !dests.empty() ) // If it's possible
		mCurrentPosition = dests[ aRNG() % dests.size() ];

	return *mCurrentPosition;
}

std::optional<CTableauState::coordinates> CTableauState::Move( const CTableau& aTableau, std::mt19937_64& aRNG )
{
	coordinates_vector destinations; // Save all possible aDestinations
	if( !mCurrentPosition || aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::EMPTY )		  // Empty
		return {};
	else if( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::QUEEN )		  // Queen
	{
		destinations.reserve( KQ_DESTINATIONS_COUNT() );
		MoveDiagonal( aTableau, destinations );
		MoveStraight( aTableau, destinations );
	}
	else if( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::BISHOP ) // Bishop
	{
		destinations.reserve( RB_DESTINATIONS_COUNT() );
		MoveDiagonal( aTableau, destinations );
	}
	else if( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::ROOK ) // Rook
	{
		destinations.reserve( RB_DESTINATIONS_COUNT() );
		MoveStraight( aTableau, destinations );
	}
	else if( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::KNIGHT ) // Knight
	{
		destinations.reserve( KQ_DESTINATIONS_COUNT() );
		MoveKnight( aTableau, destinations );
	}
	else if( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) == CTableau::E_PIECE_TYPE::WILDCARD ) // Rum (WILDCARD!)
	{
		destinations.reserve( aTableau.Size() );
		MoveWildcard( aTableau, destinations );
	}
	else
	{
		destinations.reserve( NUMERIC_DESTINATIONS_COUNT() );
		MoveDiagonal( aTableau, destinations, static_cast< int >( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) ) );
		MoveStraight( aTableau, destinations, static_cast< int >( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) ) );
	}
	mScore += Update( aTableau.GetPiece( ( *mCurrentPosition ).first, ( *mCurrentPosition ).second ) );
	mHistory.push_back( std::move( *mCurrentPosition ) );
	if( !destinations.empty() ) // If it's possible
		mCurrentPosition = destinations[ aRNG() % destinations.size() ];
	else
		mCurrentPosition.reset();

	return mCurrentPosition;
}

void CTableauState::SaveHistory( std::string_view aOutputFileName, const CTableau& aInitialTableau ) const
{
	std::ofstream outfile;
	outfile.open( aOutputFileName.data() );
	outfile << "Step #   (i,j)   Piece" << std::endl;
	outfile << "----------------------" << std::endl;

	unsigned int index = 0;
	for( const auto& epoch : mHistory )
		outfile << std::setw( 4 ) << index++ << "     (" << epoch.first << "," << epoch.second << ")     " << std::setw( 1 ) << CTableau::PieceToString( aInitialTableau.GetPiece( epoch.first, epoch.second ) ) << std::endl;
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for( CTableau::index i = 0; i < aInitialTableau.GetRows(); i++ )
		for( CTableau::index j = 0; j < aInitialTableau.GetRows(); j++ )
			if( aInitialTableau.GetPiece( i, j ) != CTableau::E_PIECE_TYPE::EMPTY && std::find( mHistory.cbegin(), mHistory.cend(), std::make_pair( i, j ) ) == mHistory.cend() )
				outfile << "(" << i << "," << j << "): " << CTableau::PieceToString( aInitialTableau.GetPiece( i, j ) ) << std::endl;
	outfile.close();
}

void CTableauState::MoveDiagonal( const CTableau& aTableau, coordinates_vector& aDestinations ) const
{
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - std::min( ( *mCurrentPosition ).first - 0, ( *mCurrentPosition ).second - 0 ),
		( *mCurrentPosition ).second - std::min( ( *mCurrentPosition ).first - 0, ( *mCurrentPosition ).second - 0 ) } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + std::min( 5 - ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - 0 ),
		( *mCurrentPosition ).second - std::min( 5 - ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - 0 ) } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + std::min( 5 - ( *mCurrentPosition ).first, 5 - ( *mCurrentPosition ).second ),
		( *mCurrentPosition ).second + std::min( 5 - ( *mCurrentPosition ).first, 5 - ( *mCurrentPosition ).second ) } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - std::min( ( *mCurrentPosition ).first - 0, 5 - ( *mCurrentPosition ).second ),
		( *mCurrentPosition ).second + std::min( ( *mCurrentPosition ).first - 0, 5 - ( *mCurrentPosition ).second ) } );
}

void CTableauState::MoveDiagonal( const CTableau& aTableau, coordinates_vector& aDestinations, int aSteps ) const
{
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second - aSteps } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second - aSteps } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second + aSteps } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second + aSteps } );
}

void CTableauState::MoveStraight( const CTableau& aTableau, coordinates_vector& aDestinations ) const
{
	AppendDestination( aTableau, aDestinations, { 0, ( *mCurrentPosition ).second } );
	AppendDestination( aTableau, aDestinations, { 5, ( *mCurrentPosition ).second } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first, 0 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first, 5 } );
}

void CTableauState::MoveStraight( const CTableau& aTableau, coordinates_vector& aDestinations, int aSteps ) const
{
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + aSteps, ( *mCurrentPosition ).second } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - aSteps, ( *mCurrentPosition ).second } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first, ( *mCurrentPosition ).second + aSteps } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first, ( *mCurrentPosition ).second - aSteps } );
}

void CTableauState::MoveKnight( const CTableau& aTableau, coordinates_vector& aDestinations ) const
{
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - 1, ( *mCurrentPosition ).second - 2 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - 1, ( *mCurrentPosition ).second + 2 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + 1, ( *mCurrentPosition ).second - 2 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + 1, ( *mCurrentPosition ).second + 2 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - 2, ( *mCurrentPosition ).second - 1 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first - 2, ( *mCurrentPosition ).second + 1 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + 2, ( *mCurrentPosition ).second - 1 } );
	AppendDestination( aTableau, aDestinations, { ( *mCurrentPosition ).first + 2, ( *mCurrentPosition ).second + 1 } );
}

void CTableauState::MoveWildcard( const CTableau& aTableau, coordinates_vector& aDestinations ) const
{
	for( CTableau::index i = 0; i < aTableau.GetRows(); i++ )
		for( CTableau::index j = 0; j < aTableau.GetRows(); j++ )
			AppendDestination( aTableau, aDestinations, { i, j } );
	return;
}

void CTableauState::AppendDestination( const CTableau& aTableau, coordinates_vector& aDestinations, const coordinates& aDestination ) const
{
	if( *mCurrentPosition != aDestination && aTableau.IsInside( aDestination.first, aDestination.second ) && aTableau.GetPiece( aDestination.first, aDestination.second ) != CTableau::E_PIECE_TYPE::EMPTY &&
		std::find( mHistory.cbegin(), mHistory.cend(), aDestination ) == mHistory.cend() )
		aDestinations.push_back( aDestination );
}

unsigned short CTableauState::Update( const piece_type& aPiece )
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