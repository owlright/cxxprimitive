cmake_minimum_required(VERSION 3.19.0)
project(primitive VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/$<CONFIG>")
set(CMAKE_CXX_STANDARD 17)
if(MSVC)
    add_compile_options("/utf-8")
endif()

add_library(primitive STATIC rasterizer.cpp triangle.cpp line.cpp image.cpp drawingboard.cpp Worker.cpp)

add_subdirectory(test)

add_executable(lineart lineart.cpp)
target_link_libraries(lineart PRIVATE primitive)

add_executable(testDrawLine testDrawLine.cpp)
target_link_libraries(testDrawLine PRIVATE primitive)