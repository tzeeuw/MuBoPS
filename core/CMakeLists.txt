set(SOURCE_FILES
    src/main.cpp)

add_executable(core ${SOURCE_FILES})


target_link_libraries(core PUBLIC glfw glad)