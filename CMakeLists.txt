cmake_minimum_required(VERSION 3.17)
project(Laborna5)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_CXX_STANDARD 20)

add_executable(Laborna5 main.cpp structures.h "QuadTree.h" "QuadTree.cpp")