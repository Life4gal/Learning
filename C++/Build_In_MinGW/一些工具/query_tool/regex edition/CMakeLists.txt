# cmake_minimum_required(VERSION <specify CMake version here>)
project(CLionProject)

set(CMAKE_CXX_STANDARD 11)

add_executable(query_regex main.cpp inc/CommandLineQuery.cpp inc/CommandLineQuery.h inc/MyQuery.h inc/MyQuery.cpp inc/ParamsAnalyzer.cpp inc/ParamsAnalyzer.h )
