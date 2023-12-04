#pragma once

#include "CTableauState.h"
#include <list>
#include <mutex>
#include <atomic>

namespace blacksmith
{

class CResult
{
public:
	using coordinates = CTableauState::coordinates;
	using history = std::list<coordinates>;

	explicit CResult( const CTableauState& aInitialTableauState );

	const history& GetHistory() const;
	const CTableauState& GetTableauState() const;
	const std::atomic_ulong& GetCounter() const;

	void FindBest( const unsigned int& aNTries, const CTableau& aTableau, std::mt19937_64& aRNG, const bool aSpeed );

	bool IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const;

	void SaveHistory( std::string_view aOutputFileName, const CTableau& aInitialTableau ) const;

private:
	history mHistory;
	CTableauState mTableauState;
	std::mutex mMutex;
	std::atomic_ulong mCounter;
};

};