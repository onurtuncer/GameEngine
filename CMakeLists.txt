cmake_minimum_required(VERSION 3.22)

# Set the project name
project(Hazel)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable target
add_library(Hazel
    src/Application.cpp
)

# Include the current directory for header files
target_include_directories(Hazel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hazel)

add_subdirectory(test)