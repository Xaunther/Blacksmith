#include "CResult.h"

namespace blacksmith
{

const CResult::history& CResult::GetHistory() const
{
	return mHistory;
}

const CTableau& CResult::GetTableau() const
{
	return mTableau;
}
const std::atomic_ulong& CResult::GetCounter() const
{
	return mCounter;
}

void CResult::FindBest( const CTableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries, std::mt19937_64& aRNG, const bool aSpeed )
{
	const auto& targetPieces = aSpeed ? aInitialBoard.CountPieces() : MAXPOS + 1;
	for( ; mCounter < aNTries && mHistory.size() < targetPieces; mCounter++ ) // Iterate many times
	{
		history posHistory = { aInitialPosition };
		CTableau board{ aInitialBoard };
		// Randomize position if requested
		if( posHistory.back().first == ROW || posHistory.back().second == COL )
			posHistory.back() = aInitialBoard.Randomize( posHistory.back() );
		while( posHistory.back().first > -1 ) // When no moves available, it is -1
			posHistory.emplace_back( board.Move( posHistory.back(), aRNG ) );
		posHistory.pop_back();
		std::lock_guard lock( mMutex );
		if( IsBetterResult( posHistory.size(), board.GetScore() ) ) // New record!
		{
			mHistory = std::move( posHistory );
			mTableau = std::move( board );
		}
	}
}

bool CResult::IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const
{
	return ( aCountHits > mHistory.size() ) || ( aCountHits == mHistory.size() && aScore > mTableau.GetScore() );
}

};