#include "CResult.h"

#include <fstream>
#include <iomanip>

namespace blacksmith
{

CResult::CResult( const CTableauState& aInitialTableauState ) :
	mTableauState( aInitialTableauState )
{
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
	for( ; mCounter < aNTries && mTableauState.GetHistory().size() < targetPieces; mCounter++ ) // Iterate many times
	{
		auto tableauState = initialTableauState;
		if( !initialTableauState.GetCurrentPosition() )
			tableauState.SetCurrentPositionAtRandom( aTableau, aRNG );
		history posHistory{ *initialTableauState.GetCurrentPosition() };
		while( const auto& nextPosition = tableauState.Move( aTableau, aRNG ) )
			posHistory.emplace_back( *nextPosition );
		std::lock_guard lock( mMutex );
		if( IsBetterResult( posHistory.size(), tableauState.GetScore() ) ) // New record!
			mTableauState = std::move( tableauState );
	}
}

bool CResult::IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const
{
	return ( aCountHits > mTableauState.GetHistory().size() ) || ( aCountHits == mTableauState.GetHistory().size() && aScore > mTableauState.GetScore() );
}

};
