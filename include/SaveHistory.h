#pragma once

#include "CTableau.h"
#include <list>

namespace blacksmith
{

using history = std::list<CTableau::destination>;
void SaveHistory( const history& aHistory, const CTableau& aTableau, const CTableau& aInitialTableau );

};
