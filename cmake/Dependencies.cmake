# cmake/Dependencies.cmake
include_guard()

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

# Close to default values
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

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

# Include ImGui + backends + SDL2 headers
target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends

    # SDL2 include dirs (same ones you already use for Hazel)
    ${sdl2_SOURCE_DIR}/include
    ${sdl2_BINARY_DIR}/include
    ${sdl2_BINARY_DIR}/include/SDL2
    ${sdl2_BINARY_DIR}/include-config-debug/SDL2    # for Debug config
)