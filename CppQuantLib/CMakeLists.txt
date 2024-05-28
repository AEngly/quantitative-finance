cmake_minimum_required(VERSION 3.10)
project(CppQuantLib)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

# Add the main executable
add_executable(CppQuantLib
    src/main.cpp
    src/BlackScholesFD.cpp
    src/Utilities.cpp
)

# Add the test executable
add_executable(TestBlackScholesFD
    tests/TestBlackScholesFD.cpp
    src/BlackScholesFD.cpp
    src/Utilities.cpp
)

# Enable testing
enable_testing()
add_test(NAME TestBlackScholesFD COMMAND TestBlackScholesFD)