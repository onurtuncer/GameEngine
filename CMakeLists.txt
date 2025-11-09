cmake_minimum_required(VERSION 3.26)

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

# --- SDL2 (new) ---------------------------------------------------------------
# Official CMake config exports SDL2::SDL2 and optionally SDL2::SDL2main.
# We only link SDL2::SDL2 because we own main().
FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.30.9
)
FetchContent_MakeAvailable(sdl2)

# --- GLAD (OpenGL function loader) -------------------------------
# This builds the glad target from source (no pre-generated files needed).
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v0.1.36
)
FetchContent_MakeAvailable(glad)

# Optional: OpenGL (for glad + your renderer)
find_package(OpenGL REQUIRED)  # provides OpenGL::GL on most platforms

# Add the executable target
add_library(Hazel
                    src/Core/Application.cpp
                    src/Core/Core.cpp
                    src/Core/Log.cpp
                    src/Platform/WindowsWindow.cpp  "Hazel/Hazel/Core/Platform.h")

# Include the current directory for header files
target_include_directories(Hazel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hazel)

target_include_directories(Hazel PRIVATE
   ${sdl2_SOURCE_DIR}/include
   ${glad_SOURCE_DIR}/include
)

target_link_libraries(Hazel PUBLIC spdlog::spdlog_header_only 
                            PRIVATE   SDL2::SDL2
    glad
    OpenGL::GL)         # ensures opengl32 on Windows / proper GL on other OSes

if (MSVC)
  target_compile_definitions(Hazel PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()


add_subdirectory(test)