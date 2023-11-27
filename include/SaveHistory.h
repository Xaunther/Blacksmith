// Function to save the best combination
#ifndef SaveHistory_h
#define SaveHistory_h

#include "tableau.h"
#include <list>

namespace blacksmith
{

using history = std::list<tableau::destination>;
void SaveHistory( const history& aHistory, const tableau& aTableau, const tableau& aInitialTableau );

};

#endif
