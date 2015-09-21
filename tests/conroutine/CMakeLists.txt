cmake_minimum_required(VERSION 2.8)

project (ConroutineTests)

set(CMAKE_CXX_FLAGS " -L/Users/bodeng/local/lib/ -std=c++11")

include_directories(../../conroutine/)
include_directories(../../conroutine/pt-1.4/)

# add_executable (ConroutineUnitTests task.cc ../main.cpp)
add_executable (ConroutineUnitTests task.cc)

