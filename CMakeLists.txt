cmake_minimum_required(VERSION 3.22)
project(Blacksmith VERSION 1.0 DESCRIPTION "A command line solver for the blacksmithing puzzle from the game Puzzle Pirates"
	HOMEPAGE_URL "https://github.com/Xaunther/Blacksmith" LANGUAGES CXX)

# C++ 20 required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Names of relevant directories
set(BLACKSMITH_DIR ${CMAKE_SOURCE_DIR})
set(BLACKSMITH_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(BLACKSMITH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(BLACKSMITH_LIBS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -Os")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	set(CMAKE_CXX_FLAGS "-W4 -WX -EHsc -Os")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-Os")

# Function to add a new object module
function(add_blacksmith_object_module MODULE_NAME SOURCES)
	add_Library(${MODULE_NAME}Objects OBJECT ${SOURCES} ${ARGN})
	set_target_properties(${MODULE_NAME}Objects PROPERTIES
		POSITION_INDEPENDENT_CODE true
		ARCHIVE_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR})
	MESSAGE(STATUS "Added object module ${MODULE_NAME}")
endfunction()

# Function to add a new module
function(add_blacksmith_module MODULE_NAME MODULE_TYPE SOURCES)
	if(MODULE_TYPE STREQUAL SHARED)
		set(MODULE_SUFFIX "Shared")
	elseif(MODULE_TYPE STREQUAL STATIC)
		set(MODULE_SUFFIX "Static")
	else()
		MESSAGE(FATAL_ERROR "Module type ${MODULE_TYPE} not recognized. Only 'STATIC' and 'SHARED' are allowed.")
	endif()

	add_Library(${MODULE_NAME}${MODULE_SUFFIX} ${MODULE_TYPE} ${SOURCES} ${ARGN})
	set_target_properties(${MODULE_NAME}${MODULE_SUFFIX} PROPERTIES
		POSITION_INDEPENDENT_CODE true
		ARCHIVE_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR}
		LIBRARY_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${BLACKSMITH_LIBS_DIR})
	target_link_libraries(${MODULE_NAME}${MODULE_SUFFIX})
	MESSAGE(STATUS "Added ${MODULE_TYPE} module ${MODULE_NAME}")
endfunction()

# List of source files
include(FileList.cmake)

# Include directories
include_directories(${BLACKSMITH_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Link directories
link_directories(${BLACKSMITH_LIBS_DIR})

# Build Blacksmith solver
add_blacksmith_object_module(${PROJECT_NAME}Core "${BLACKSMITH_CORE_SOURCE}")
set(BLACKSMITH_CORE_OBJECTS $<TARGET_OBJECTS:${PROJECT_NAME}CoreObjects>)
set(BLACKSMITH_OBJECTS ${BLACKSMITH_CORE_OBJECTS})

# Final build step. All objects must have been appended to BLACKSMITH_OBJECTS.
add_blacksmith_module(${PROJECT_NAME}Core STATIC ${BLACKSMITH_OBJECTS})

if(LIB_BLACKSMITH_SHARED)
	add_blacksmith_module(${PROJECT_NAME}Core SHARED ${BLACKSMITH_OBJECTS})
endif()

# Compile solver binary
add_executable(Blacksmith ${CMAKE_SOURCE_DIR}/src/main.cpp ${BLACKSMITH_OBJECTS})