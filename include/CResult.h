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
	using history = std::list<CTableau::destination>;

	const history& GetHistory() const;
	const CTableauState& GetTableauState() const;
	const std::atomic_ulong& GetCounter() const;

	void FindBest( const CTableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries, std::mt19937_64& aRNG, const bool aSpeed );

	bool IsBetterResult( const history::size_type& aCountHits, const unsigned short& aScore ) const;

	void SaveHistory( std::string_view aOutputFileName, const CTableau& aInitialTableau ) const;

private:
	history mHistory;
	CTableauState mTableauState;
	std::mutex mMutex;
	std::atomic_ulong mCounter;
};

};