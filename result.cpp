#include "result.h"
#include <iostream>

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

void result::FindBest( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries, std::mt19937_64& aRNG )
{
	const int N_pieces = aInitialBoard.CountPieces(); // Count how many pieces there are (max number of steps one can achieve)

	for( unsigned long i = 0; i < aNTries && mHistory.size() < N_pieces; i++ ) // Iterate many times
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
		if( poshistory.size() > mHistory.size() ) // New record!
		{
			mHistory = std::move( poshistory );
			mTableau = std::move( board );
		}
	}
}

};