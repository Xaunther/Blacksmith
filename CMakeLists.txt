cmake_minimum_required(VERSION 3.0.0)
project(Blacksmith)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR}/include)

file(GLOB SOURCES "src/*.cpp")

add_executable(Blacksmith ${SOURCES})
