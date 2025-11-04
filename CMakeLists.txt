cmake_minimum_required(VERSION 3.22)

# Set the project name
project(Hazel LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

# Pin a specific release tag of spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1        # <-- pin the version you want
  GIT_SHALLOW    TRUE
)
# Header-only: avoid building anything
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH   OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(spdlog)

# Add the executable target
add_library(Hazel
    src/Application.cpp
    src/Log.cpp
 "Hazel/Hazel/Events/Event.h" "Hazel/Hazel/Events/ApplicationEvent.h" "Hazel/Hazel/Events/KeyEvent.h" "Hazel/Hazel/Events/MouseEvent.h")

# Include the current directory for header files
target_include_directories(Hazel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hazel)
target_link_libraries(Hazel PUBLIC spdlog::spdlog_header_only)

add_subdirectory(test)