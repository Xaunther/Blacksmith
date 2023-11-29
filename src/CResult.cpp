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

void CResult::FindBest( const unsigned int& aNTries, std::mt19937_64& aRNG, const bool aSpeed )
{
	const auto initialTableauState = mTableauState;
	const auto& targetPieces = aSpeed ? initialTableauState.GetTableau().CountPieces() : MAXPOS + 1;
	for( ; mCounter < aNTries && mHistory.size() < targetPieces; mCounter++ ) // Iterate many times
	{
		history posHistory = { initialTableauState.GetCurrentPosition() };
		auto tableauState = initialTableauState;
		// Randomize position if requested
		if( posHistory.back().first == ROW || posHistory.back().second == COL )
			posHistory.back() = tableauState.Randomize();
		while( posHistory.back().first > -1 ) // When no moves available, it is -1
			posHistory.emplace_back( tableauState.Move( aRNG ) );
		posHistory.pop_back();
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
		outfile << std::setw( 4 ) << index++ << "     (" << epoch.first << "," << epoch.second << ")     " << std::setw( 1 ) << aInitialTableau.GetPieces()[ epoch.first ][ epoch.second ] << std::endl;
	// Write also remaining pieces
	outfile << "----------------------" << std::endl;
	outfile << "   Remaining Pieces   " << std::endl;
	outfile << "----------------------" << std::endl;
	for( int i = 0; i < ROW; i++ )
		for( int j = 0; j < COL; j++ )
			if( mTableauState.GetTableau().GetPieces()[ i ][ j ] != "E" )
				outfile << "(" << i << "," << j << "): " << mTableauState.GetTableau().GetPieces()[ i ][ j ] << std::endl;
	outfile.close();
}


};