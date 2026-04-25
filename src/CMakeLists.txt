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
target_sources(glad PRIVATE ../vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC ../vendor/glad/include)

# stb_image (compile from vendor)
add_library(stb_image)
target_sources(stb_image PRIVATE ../vendor/stb_image/stb_image.cpp)
target_include_directories(stb_image PUBLIC ../vendor/stb_image)

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
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imguizmo/ImGuizmo.cpp
)
target_include_directories(ImGuizmo PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/imguizmo
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
