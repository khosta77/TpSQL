cmake_minimum_required(VERSION 3.16)
project(TpSQL)

set(CMAKE_CXX_STANDARD 14)

add_executable(TpSQL main.cpp
        ./project/include/Table.h
        ./project/src/Table.cpp
        ./project/include/Tcmd.h
        ./project/src/Tcmd.cpp
)
