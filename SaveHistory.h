// Function to save the best combination
#ifndef SaveHistory_h
#define SaveHistory_h

#include "tableau.h"
#include <utility>
#include <array>

using history = std::array<tableau::destination, maxpos + 1>;
void SaveHistory( const history& aHistory, const int aSteps, tableau& aTableau );

#endif
