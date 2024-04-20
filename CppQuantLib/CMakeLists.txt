cmake_minimum_required(VERSION 3.10)
project(CppQuantLib)

# Add executable target
add_executable(CppQuantLib
    src/main.cpp
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)