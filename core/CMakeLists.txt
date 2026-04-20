set(SOURCE_FILES
    src/main.cpp
    src/ClassicalBody.cpp
    src/Simulation.cpp)

add_executable(core ${SOURCE_FILES})


target_include_directories(core PUBLIC
    ${CMAKE_SOURCE_DIR}/core/include
)

target_link_libraries(core PUBLIC glfw glad glm)
