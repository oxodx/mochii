add_library(mochii STATIC)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

target_sources(mochii PRIVATE ${SOURCES})

# Precompiled header
target_precompile_headers(mochii PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mzpch.h
)

# Include directories
target_include_directories(mochii SYSTEM PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/glad/include
)

target_include_directories(mochii PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(mochii PRIVATE
    ${glfw_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${spdlog_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb_image
    ${fmt_SOURCE_DIR}/include
)

# Link libraries
target_link_libraries(mochii PUBLIC
    glad
    glfw
    ImGui
    stb_image
    opengl32
    fmt
)

# EnTT is header-only but linking its target propagates include paths and compile defs.
if(TARGET EnTT::EnTT)
    target_link_libraries(mochii PUBLIC EnTT::EnTT)
elseif(TARGET entt::entt)
    target_link_libraries(mochii PUBLIC entt::entt)
endif()

# Preprocessor definitions
target_compile_definitions(mochii PRIVATE
    MI_BUILD_DLL
    GLFW_INCLUDE_NONE
    _CRT_SECURE_NO_WARNINGS
)

# Configuration-specific definitions
target_compile_definitions(mochii PRIVATE
    $<$<CONFIG:Debug>:MI_DEBUG>
    $<$<CONFIG:Release>:MI_RELEASE>
    $<$<CONFIG:Distribution>:MI_DIST>
)
