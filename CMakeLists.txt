cmake_minimum_required(VERSION 3.20)
project(Mochii)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for clangd and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Fetch external dependencies
include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 3.4
)

# ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG docking
)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog
    GIT_TAG v1.17.0
)

# fmt (required by spdlog)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG 12.1.0
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 1.0.3
)

# entt
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt
    GIT_TAG v3.16.0
)

# stb_image (header-only, we'll add it manually)
# For stb_image, we'll create a simple wrapper library

# Make content available
FetchContent_MakeAvailable(glfw spdlog glm fmt entt)

# Handle ImGui separately (not CMake-based)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_MakeAvailable(imgui)
endif()

# GLAD (build from vendor)
add_library(glad)
target_sources(glad PRIVATE vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# stb_image (compile from vendor)
add_library(stb_image)
target_sources(stb_image PRIVATE vendor/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC vendor/stb_image)

# ImGui library
add_library(ImGui)
target_sources(ImGui PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)
target_include_directories(ImGui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(ImGui PUBLIC glad)

# Static link MinGW runtime to avoid DLL dependencies
if (MINGW)
    add_link_options(-static-libgcc -static-libstdc++ -static)
endif()

# Add projects
add_subdirectory(Mochii)
add_subdirectory(MochiiEditor)
add_subdirectory(Sandbox)
