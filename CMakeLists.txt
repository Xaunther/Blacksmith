cmake_minimum_required(VERSION 3.0.0)
project(Blacksmith)

set (CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB SOURCES "*.cxx")

add_executable(Blacksmith ${SOURCES})
