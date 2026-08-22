cmake_minimum_required(VERSION 3.14)
project(mubops VERSION 0.4.0)


# --- Windows-only: make sure every target uses the DYNAMIC C runtime (/MD or /MDd) ---
# Python's own python3xx.dll is built with the dynamic runtime, and any .pyd extension
# module (like mubops_core) MUST match that, or you get "unresolved external" linker
# errors when mixing static-runtime code with dynamic-runtime code.
# This block only affects MSVC — on Linux/macOS it does nothing (there's no such split there).
if (MSVC)
    cmake_policy(SET CMP0091 NEW)   # tells CMake to actually respect the line below
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Shared GLFW and GLAD libraries for the entire project
# GLFW and GLM
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG 3.3-stable)
FetchContent_MakeAvailable(glfw)

# Force GLFW to also use the dynamic runtime (belt-and-suspenders — GLFW's own
# CMakeLists can sometimes set its own runtime library and override the global setting above).
if (MSVC)
    set_property(TARGET glfw PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

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

# Same runtime-library safety net for glad.
if (MSVC)
    set_property(TARGET glad PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

add_subdirectory(core)
add_subdirectory(sandbox)
