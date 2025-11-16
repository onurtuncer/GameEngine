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

# Generator ayarlar? (default’a yak?n tutuyoruz)
set(GLAD_PROFILE   "core" CACHE STRING "" FORCE)
set(GLAD_API       "gl=3.3" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c"     CACHE STRING "" FORCE)
set(GLAD_INSTALL   OFF     CACHE BOOL   "" FORCE)


FetchContent_MakeAvailable(glad)

# Optional: OpenGL (for glad + your renderer)
find_package(OpenGL REQUIRED)  # provides OpenGL::GL on most platforms


FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG        master        # or a specific tag like v1.91.0
)

FetchContent_MakeAvailable(imgui)

#add_library(imgui_sdl2_opengl3
#    ${imgui_SOURCE_DIR}/imgui.cpp
#    ${imgui_SOURCE_DIR}/imgui_demo.cpp
#    ${imgui_SOURCE_DIR}/imgui_draw.cpp
#    ${imgui_SOURCE_DIR}/imgui_tables.cpp
#    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
#
#    # Backends
#    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
#    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
# "Hazel/Hazel/Core/ImGuiLayer.h" "src/Core/ImGuiLayer.cpp")
#
#target_include_directories(imgui_sdl2_opengl3
#    PUBLIC
#        ${imgui_SOURCE_DIR}
#        ${imgui_SOURCE_DIR}/backends
#)

# Link against your existing SDL2 + GL targets / libs
#target_link_libraries(imgui_sdl2_opengl3
#    PUBLIC
#        SDL2-static          # or SDL2::SDL2 if using imported targets
#        glad                 # or OpenGL::GL depending on your setup
#)

#add_library(imgui STATIC
#    ${IMGUI_SOURCE_DIR}/imgui.cpp
#    ${IMGUI_SOURCE_DIR}/imgui_draw.cpp
#    ${IMGUI_SOURCE_DIR}/imgui_tables.cpp
#    ${IMGUI_SOURCE_DIR}/imgui_widgets.cpp
#    # backends etc
#)


# Add the executable target
add_library(Hazel
                    src/Core/Application.cpp
                    src/Core/Base.cpp
                    src/Core/Log.cpp
                    src/Platform/WindowsWindow.cpp  
                    src/Core/Layer.cpp 
                    src/Core/LayerStack.cpp
                    "src/Core/ImGuiLayer.cpp"
                    "src/hzpch.cpp")

# Include the current directory for header files
target_include_directories(Hazel PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Hazel)

# Enable PCH for Hazel
target_precompile_headers(Hazel
    PRIVATE
        "Hazel/Hazel/hzpch.h"    
)


target_include_directories(Hazel PRIVATE
   ${sdl2_SOURCE_DIR}/include
   ${glad_SOURCE_DIR}/include
)

target_link_libraries(Hazel PUBLIC spdlog::spdlog_header_only 
                            PRIVATE   SDL2::SDL2
    glad
    OpenGL::GL  # ensures opengl32 on Windows / proper GL on other OSes
)         

if (MSVC)
  target_compile_definitions(Hazel PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()


add_subdirectory(test)