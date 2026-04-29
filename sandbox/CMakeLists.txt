# pybind11_add_module(example
# pybind-testing/example.cpp
# )

# set_target_properties(example PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/.venv/lib/python3.13/site-packages)


set(SOURCE_FILES
    main.cpp)

add_executable(sandbox ${SOURCE_FILES})

target_link_libraries(sandbox PUBLIC glfw glad glm)