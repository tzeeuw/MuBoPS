cmake_minimum_required(VERSION 3.10)
project(mubops)

set(CMAKE_CXX_STANDARD 17)


# Shared GLFW and GLAD libraries for the entire project
# GLFW
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3-stable)
FetchContent_MakeAvailable(glfw)

# glad
add_library(glad external/glad/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external
)

add_subdirectory(core)
add_subdirectory(sandbox)