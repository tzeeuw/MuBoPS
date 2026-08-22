set(SOURCE_FILES
    src/ClassicalBody.cpp
    src/Simulation.cpp
    src/QuantumBody.cpp
    src/Renderer.cpp
    src/Octree.cpp)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Optimization flags, per compiler.
# We used to just do set(CMAKE_CXX_FLAGS_RELEASE "-O3") etc, but that's a GCC/Clang-only
# flag — on MSVC it gets silently ignored (D9002 warning) AND it wipes out CMake's own
# default flags for that build type, including the one that sets the runtime library
# (/MD vs /MT). That's what caused the "unresolved external" linker errors on Windows.
# So: give MSVC its own flags, and only apply -O3/-O0/-g on non-MSVC compilers.
if (MSVC)
    add_compile_options($<$<CONFIG:Release>:/O2> $<$<CONFIG:Debug>:/Od> $<$<CONFIG:Debug>:/Zi>)
else()
    add_compile_options($<$<CONFIG:Release>:-O3> $<$<CONFIG:Debug>:-O0> $<$<CONFIG:Debug>:-g>)
endif()


add_library(core_lib ${SOURCE_FILES})


target_include_directories(core_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/core/include
)

target_link_libraries(core_lib PUBLIC glfw glad glm)

add_executable(core
    src/main.cpp
)
add_executable(test
    tests/octreeTest.cpp
)

target_link_libraries(core PRIVATE core_lib)
target_link_libraries(test PRIVATE core_lib)

pybind11_add_module(mubops_core src/bindings.cpp)

# Force the dynamic C runtime (/MD or /MDd) on every target built here.
# This has to be explicit on EACH target — CMake doesn't propagate a runtime-library
# setting through target_link_libraries() the way it does with compile definitions.
# Without this, a target with no flags set defaults to /MT (static runtime) on MSVC,
# which is incompatible with glfw3.lib/glad.lib (built /MD) and with Python's own
# runtime — causing the exact linker errors we were chasing.
if (MSVC)
    set_property(TARGET core_lib PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET test PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set_property(TARGET mubops_core PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

target_link_libraries(mubops_core PRIVATE core_lib)
set_target_properties(mubops_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/src/mubops)
