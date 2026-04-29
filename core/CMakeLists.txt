set(SOURCE_FILES
    src/ClassicalBody.cpp
    src/Simulation.cpp
    src/QuantumBody.cpp
    src/Renderer.cpp)

add_library(core_lib ${SOURCE_FILES})


target_include_directories(core_lib PUBLIC
    ${CMAKE_SOURCE_DIR}/core/include
)

target_link_libraries(core_lib PUBLIC glfw glad glm)


add_executable(core
    src/main.cpp
)
target_link_libraries(core PRIVATE core_lib)

pybind11_add_module(mubops_core src/bindings.cpp)
target_link_libraries(mubops_core PRIVATE core_lib)
set_target_properties(mubops_core PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/src/mubops
                                LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/src/mubops)
