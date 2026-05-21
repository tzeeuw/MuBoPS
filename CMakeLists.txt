cmake_minimum_required(VERSION 3.14)
project(mubops VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Shared GLFW and GLAD libraries for the entire project
# GLFW and GLM
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3-stable)
FetchContent_MakeAvailable(glfw)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git)
FetchContent_MakeAvailable(glm)
FetchContent_Declare(pybind11 GIT_REPOSITORY https://github.com/pybind/pybind11.git GIT_TAG stable)
FetchContent_MakeAvailable(pybind11)


find_package(Python REQUIRED COMPONENTS Interpreter Development)

message(STATUS "Using Python: ${Python_EXECUTABLE}")

# glad
add_library(glad external/glad/glad.c)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/external
)

add_subdirectory(core)
add_subdirectory(sandbox)
