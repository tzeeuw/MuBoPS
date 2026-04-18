add_executable(sandbox
    opengl-testing/twoTriangles.cpp
)

target_link_libraries(sandbox PUBLIC
    glfw
    glad
)