cmake_minimum_required(VERSION 3.15)
project(CPPTest)

set(CMAKE_CXX_STANDARD 11)

add_executable(query_iterator main.cpp MyQuery.h ParamsAnalyzer.h)