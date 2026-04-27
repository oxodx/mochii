add_executable(MochiiEditor)

# Source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

target_sources(MochiiEditor PRIVATE ${SOURCES})

# Include directories
target_include_directories(MochiiEditor PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../Mochii/src
    ${glfw_SOURCE_DIR}/include
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${spdlog_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/stb_image
    ${glm_SOURCE_DIR}
)

# Link libraries
target_link_libraries(MochiiEditor PRIVATE
    mochii
    glfw
    ImGui
    ImGuizmo
)

if (WIN32)
    target_link_libraries(MochiiEditor PRIVATE
        urlmon
        shell32
        mfplat
        mfreadwrite
        mfuuid
        ole32
    )
endif()

# Preprocessor definitions
target_compile_definitions(MochiiEditor PRIVATE
    $<$<CONFIG:Debug>:MI_DEBUG>
    $<$<CONFIG:Release>:MI_RELEASE>
    $<$<CONFIG:Distribution>:MI_DIST>
)
