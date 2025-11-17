# cmake/Dependencies.cmake
include_guard()

include(FetchContent)

# ---------------- spdlog ----------------
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.14.1
  GIT_SHALLOW    TRUE
)

# Header-only: avoid building extras
set(SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTS   OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH   OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(spdlog)

# ---------------- SDL2 ----------------
FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-2.30.9
)
FetchContent_MakeAvailable(sdl2)
# Provides SDL2::SDL2

# ---------------- glad ----------------
FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG        v0.1.36
)

set(GLAD_PROFILE   "core" CACHE STRING "" FORCE)
set(GLAD_API       "gl=3.3" CACHE STRING "" FORCE)
set(GLAD_GENERATOR "c"     CACHE STRING "" FORCE)
set(GLAD_INSTALL   OFF     CACHE BOOL   "" FORCE)

FetchContent_MakeAvailable(glad)
# Provides glad target and glad includes

# ---------------- OpenGL ----------------
find_package(OpenGL REQUIRED)  # OpenGL::GL

# ---------------- ImGui -----------------
# (fetched, but you can decide how to wrap it in a target later)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(imgui)
