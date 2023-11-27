# Core source files
SET(BLACKSMITH_CORE_SOURCE
	src/result.cpp
	src/SaveHistory.cpp
	src/tableau.cpp
)

# Core internal headers
SET(BLACKSMITH_CORE_INTERNAL_HEADERS)

# Core external headers
SET(BLACKSMITH_CORE_EXTERNAL_HEADERS
	include/Constants.h
	include/result.h
	include/SaveHistory.h
	include/tableau.h
)

# Source files
SET(BLACKSMITH_SOURCE ${BLACKSMITH_CORE_SOURCE})

# Internal headers
SET(BLACKSMITH_INTERNAL_HEADERS ${BLACKSMITH_CORE_INTERNAL_HEADERS})

# External headers
SET(BLACKSMITH_EXTERNAL_HEADERS ${BLACKSMITH_CORE_EXTERNAL_HEADERS})