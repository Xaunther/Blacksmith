#include "tableau.h"
#include <list>

namespace blacksmith
{

class result
{
public:
	using history = std::list<tableau::destination>;

	const history& GetHistory() const;
	const tableau& GetTableau() const;

	void FindBest( const tableau& aInitialBoard, const history::value_type& aInitialPosition, const unsigned int& aNTries );

private:
	history mHistory;
	tableau mTableau;
};

};