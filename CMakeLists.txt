cmake_minimum_required(VERSION 3.26)

# Set the project name
project(Hazel LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(Dependencies)

# Add the executable target
add_library(Hazel
                    src/Core/Application.cpp
                    src/Core/Base.cpp
                    src/Core/Log.cpp
                    src/Platform/WindowsWindow.cpp  
                    src/Core/Layer.cpp 
                    src/Core/LayerStack.cpp
                    "src/Core/ImGuiLayer.cpp"
                    "src/hzpch.cpp" 
                    "src/Renderer/Renderer.cpp" "Hazel/Hazel/Core/Timestep.h")

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
   ${imgui_SOURCE_DIR}
)

target_link_libraries(Hazel PUBLIC spdlog::spdlog_header_only 
                               SDL2::SDL2
    glad
    OpenGL::GL  # ensures opengl32 on Windows / proper GL on other OSes
    imgui
)         

if (MSVC)
  target_compile_definitions(Hazel PRIVATE WIN32_LEAN_AND_MEAN NOMINMAX)
endif()


add_subdirectory(test)