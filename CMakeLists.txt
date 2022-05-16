cmake_minimum_required(VERSION 3.16)
project(TpSQL)

set(CMAKE_CXX_STANDARD 14)

#file(GLOB SOURCES
#        header-folder/*.h
#        source-folder/*.cpp
#        )

add_executable(TpSQL main.cpp
        ./project/include/Table.h
        ./project/include/Tcmd.h
        ./project/include/auxiliary_func.h
        ./project/src/auxiliary_func.cpp
        ./project/src/Table.cpp)
