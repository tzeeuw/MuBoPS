set(SOURCE_FILES
    src/ClassicalBody.cpp
    src/Simulation.cpp
    src/QuantumBody.cpp
    src/Renderer.cpp
    src/Octree.cpp)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
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
target_link_libraries(mubops_core PRIVATE core_lib)
set_target_properties(mubops_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/src/mubops)
