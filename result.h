#include "tableau.h"
#include <list>
#include <mutex>

namespace blacksmith
{

class result
{
public:
	using history = std::list<tableau::destination>;

	const history& GetHistory() const;
	const tableau& GetTableau() const;
	const std::atomic_ulong& GetCounter() const;

	void FindBest( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries, std::mt19937_64& aRNG );

private:
	history mHistory;
	tableau mTableau;
	std::mutex mMutex;
	std::atomic_ulong mCounter;
};

};