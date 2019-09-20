cmake_minimum_required(VERSION 3.0.0)
project(Blacksmith)

include_directories(${CMAKE_SOURCE_DIR})

file(GLOB SOURCES "*.cxx")

add_executable(Blacksmith main.cpp)