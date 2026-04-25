# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if (EXISTS "${LOC_PATH}")
    message(FATAL_ERROR "You cannot build in the source directory (or any directory with a CMakeLists.txt). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)
set(CMAKE_WARN_DEPRECATED FALSE)
project(mochii VERSION 0.1.1 LANGUAGES C CXX
    HOMEPAGE_URL "https://github.com/oxodx/mochii")

include(cmake/CPM.cmake)
# Use a persistent source cache so CPM downloads survive --fresh rebuilds.
# Override with -DCPM_SOURCE_CACHE=<path> or the CPM_SOURCE_CACHE env var.
if (NOT CPM_SOURCE_CACHE AND NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_SOURCE_CACHE "${PROJECT_SOURCE_DIR}/.cpm_cache" CACHE PATH "CPM source cache directory")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(MOCHII_INCLUDE_ROOT "${PROJECT_SOURCE_DIR}/src")

# Split parts to cmake/
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Helper function to declare cache variables to control mochii options
macro (set_option variable_name help_text default_value possible_values)
    set("${variable_name}" "${default_value}" CACHE STRING "${help_text}")
    set_property(CACHE ${variable_name} PROPERTY STRINGS "${possible_values}")
endmacro ()

# Options to control which third party libraries are used with mochii
set_option(MOCHII_GUI_LIBRARY "GUI library. Choices: 'imgui' or 'none'" "imgui" "imgui;none")
set_option(MOCHII_WINDOW_LIBRARY "Window library. Choices: 'glfw' or 'none'" "glfw" "glfw;none")
set_option(MOCHII_GRAPHICS_LIBRARY "Graphics API. Choices: 'opengl' or 'none'" "opengl" "opengl;none")

# These are in cmake/ directory
message("Compiler = ${CMAKE_CXX_COMPILER_ID}")
include(${CMAKE_CXX_COMPILER_ID})

if (WIN32)
    message("Detected Windows platform")
    set(MOCHII_TARGET_OS_WINDOWS TRUE)
    add_definitions(-DMOCHII_OS_WINDOWS)
else()
    message(FATAL_ERROR "OS not supported ${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
endif()

# Dependencies
message("Fetching dependencies")
CPMAddPackage(NAME fmt  GIT_TAG 12.1.0  VERSION 12.1.0  GIT_SHALLOW TRUE
    GITHUB_REPOSITORY fmtlib/fmt)
CPMAddPackage(NAME spdlog  GIT_TAG v1.17.0  VERSION 1.17.0  GIT_SHALLOW TRUE
    GITHUB_REPOSITORY gabime/spdlog  OPTIONS "SPDLOG_FMT_EXTERNAL ON")
CPMAddPackage(NAME glm  GIT_TAG 1.0.3  VERSION 1.0.3  GIT_SHALLOW TRUE
    GITHUB_REPOSITORY g-truc/glm)
CPMAddPackage(NAME entt  GIT_TAG v3.16.0  VERSION 3.16.0  GIT_SHALLOW TRUE
    GITHUB_REPOSITORY skypjack/entt)
CPMAddPackage(NAME yaml-cpp  GIT_TAG yaml-cpp-0.9.0  VERSION 0.9.0  GIT_SHALLOW TRUE
    GITHUB_REPOSITORY jbeder/yaml-cpp)
add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
if(${MOCHII_GRAPHICS_LIBRARY} STREQUAL "opengl")
    message(STATUS "Mochii configured to use OpenGL as graphics API.")
    set(MOCHII_GRAPHICS_API_OPENGL TRUE)
    add_definitions(-DMOCHII_GRAPHICS_LIBRARY_OPENGL)
    find_package(OpenGL REQUIRED)
elseif(${MOCHII_GRAPHICS_LIBRARY} STREQUAL "none")
    message(STATUS "Mochii configured to use none as graphics API (headless).")
    set(MOCHII_GRAPHICS_API_NONE TRUE)
    add_definitions(-DMOCHII_GRAPHICS_LIBRARY_NONE)
endif()
if(${MOCHII_WINDOW_LIBRARY} STREQUAL "glfw")
    CPMAddPackage(NAME glfw  GIT_TAG 3.4  VERSION 3.4  GIT_SHALLOW TRUE
        GITHUB_REPOSITORY glfw/glfw  OPTIONS
                "BUILD_SHARED_LIBS OFF"
                "GLFW_BUILD_EXAMPLES OFF"
                "GLFW_BUILD_TESTS OFF"
                "GLFW_BUILD_DOCS OFF"
                "GLFW_INSTALL OFF"
                "GLFW_VULKAN_STATIC OFF")
    target_compile_options(glfw PRIVATE -w)
    message(STATUS "Mochii configured to use GLFW as windowing API.")
    set(MOCHII_WINDOW_API_GLFW TRUE)
    add_definitions(-DMOCHII_WINDOW_LIBRARY_GLFW)
elseif(${MOCHII_WINDOW_LIBRARY} STREQUAL "none")
    message(STATUS "Mochii configured to use none as windowing API (headless).")
    set(MOCHII_WINDOW_API_NONE TRUE)
    add_definitions(-DMOCHII_WINDOW_LIBRARY_NONE)
endif()
if(${MOCHII_GUI_LIBRARY} STREQUAL "imgui")
    CPMAddPackage(NAME imgui  GIT_TAG docking  VERSION docking  GIT_SHALLOW TRUE
        GITHUB_REPOSITORY ocornut/imgui)
    FetchContent_MakeAvailable(imgui)
    message(STATUS "Mochii configured to use ImGui for GUI.")
    set(MOCHII_GUI_API_IMGUI TRUE)
    add_definitions(-DMOCHII_GUI_LIBRARY_IMGUI)
elseif(${MOCHII_GUI_LIBRARY} STREQUAL "none")
    message(STATUS "Mochii configured to use none for GUI.")
    set(MOCHII_GUI_API_NONE TRUE)
    add_definitions(-DMOCHII_GUI_LIBRARY_NONE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ImGui math operators for ImGuizmo
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

# Export compile commands for clangd and other tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

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

# ImGuizmo library (from vendor)
add_library(ImGuizmo)
target_sources(ImGuizmo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imguizmo/ImGuizmo.cpp
)
target_include_directories(ImGuizmo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imguizmo
    ${imgui_SOURCE_DIR}
)

# Static link MinGW runtime to avoid DLL dependencies
if (MINGW)
    add_link_options(-static-libgcc -static-libstdc++ -static)
endif()

# Add projects
add_subdirectory(Mochii)
add_subdirectory(MochiiEditor)
add_subdirectory(Sandbox)
