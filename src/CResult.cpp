#include "CResult.h"

#include <fstream>
#include <iomanip>

namespace blacksmith
{

CResult::CResult( const CTableauState& aInitialTableauState ) :
	mTableauState( aInitialTableauState )
{
}

const CResult::history& CResult::GetHistory() const
{
	return mHistory;
}

const CTableauState& CResult::GetTableauState() const
{
	return mTableauState;
}
const std::atomic_ulong& CResult::GetCounter() const
{
	return mCounter;
}

void CResult::FindBest( const unsigned int& aNTries, const CTableau& aTableau, std::mt19937_64& aRNG, const bool aSpeed )
{
	const auto initialTableauState = mTableauState;
	const auto& targetPieces = aTableau.CountPieces() + ( aSpeed ? 0 : 1 );
	for( ; mCounter < aNTries && mHistory.size() < targetPieces; mCounter++ ) // Iterate many times
	{
		auto tableauState = initialTableauState;
		history posHistory{ initialTableauState.GetCurrentPosition().value_or( tableauState.SetCurrentPositionAtRandom( aTableau, aRNG ) ) };
		while( const auto& nextPosition = tableauState.Move( aTableau, aRNG ) )
			posHistory.emplace_back( *nextPosition );
		std::lock_guard lock( mMutex );
		if( IsBetterResult( posHistory.size(), tableauState.GetScore() ) ) // New record!
		{
			mHistory = std::move( posHistory );
			mTableauState = std::move( tableauState );
		}
	}
}

bool CResult::IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const
{
	return ( aCountHits > mHistory.size() ) || ( aCountHits == mHistory.size() && aScore > mTableauState.GetScore() );
}

void CResult::SaveHistory( std::string_view aOutputFileName, const CTableau& aInitialTableau ) const
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
			if( aInitialTableau.GetPiece( i, j ) != CTableau::E_PIECE_TYPE::EMPTY && std::find( mTableauState.GetHistory().cbegin(), mTableauState.GetHistory().cend(), std::make_pair( i, j ) ) == mTableauState.GetHistory().cend() )
				outfile << "(" << i << "," << j << "): " << CTableau::PieceToString( aInitialTableau.GetPiece( i, j ) ) << std::endl;
	outfile.close();
}


};