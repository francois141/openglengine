cmake_minimum_required(VERSION 3.12)

set(ASSIMP_BUILD_DRACO OFF CACHE BOOL "..." FORCE )



project(OpenglEngine
            VERSION 1.0.0
            DESCRIPTION "Opengl Engine"
            LANGUAGES CXX
        )

file(GLOB_RECURSE MY_SOURCES_FILES
    ${PROJECT_SOURCE_DIR}/sources/*.cpp
)

file(GLOB_RECURSE MY_HEADER_FILES
    ${PROJECT_SOURCE_DIR}/sources/*.h
)


find_package(OpenGL REQUIRED)


add_executable(
    ${PROJECT_NAME}
    ${MY_HEADER_FILES}
    ${MY_SOURCES_FILES}
)


add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/assimp)


include_directories("sources")

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
    PUBLIC external/glad/include
    PUBLIC external/glm
    PUBLIC external/assimp/include
    PUBLIC external/openal-soft
)
target_link_libraries(${PROJECT_NAME} glfw glad assimp)



add_library(imgui
    external/imgui/imgui.cpp 
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp 
    external/imgui/imgui_widgets.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    )
    

target_include_directories(imgui PUBLIC external/imgui external/imgui/backends external/glad/include)
target_link_libraries(${PROJECT_NAME} optimized imgui) 