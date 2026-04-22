cmake_minimum_required(VERSION 3.14)
project(mubops)

set(CMAKE_CXX_STANDARD 17)


# Shared GLFW and GLAD libraries for the entire project
# GLFW and GLM
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3-stable)
FetchContent_MakeAvailable(glfw)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_MakeAvailable(glm)

# glad
add_library(glad external/glad/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external
)

add_subdirectory(core)
add_subdirectory(sandbox)
