#include "result.h"

namespace blacksmith
{

const result::history& result::GetHistory() const
{
	return mHistory;
}

const tableau& result::GetTableau() const
{
	return mTableau;
}
const std::atomic_ulong& result::GetCounter() const
{
	return mCounter;
}

void result::FindBest( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries, std::mt19937_64& aRNG, const bool aSpeed )
{
	const auto& targetPieces = aSpeed ? aInitialBoard.CountPieces() : maxpos;
	for( ; mCounter < aNTries && mHistory.size() < targetPieces; mCounter++ ) // Iterate many times
	{
		history poshistory = { aInitialPosition };
		tableau board{ aInitialBoard };
		// Randomize position if requested
		if( poshistory.back().first == row || poshistory.back().second == col )
			poshistory.back() = aInitialBoard.Randomize( poshistory.back() );
		while( poshistory.back().first > -1 ) // When no moves available, it is -1
			poshistory.emplace_back( board.Move( poshistory.back(), aRNG ) );
		poshistory.pop_back();
		std::lock_guard lock( mMutex );
		if( IsBetterResult( poshistory.size(), board.GetScore() ) ) // New record!
		{
			mHistory = std::move( poshistory );
			mTableau = std::move( board );
		}
	}
}

bool result::IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const
{
	return ( aCountHits > mHistory.size() ) || ( aCountHits == mHistory.size() && aScore > mTableau.GetScore() );
}

};