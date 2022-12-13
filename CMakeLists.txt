cmake_minimum_required(VERSION 3.23)
project(MVRE_Sample)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")

add_subdirectory(MVRE)
add_subdirectory(MVRE_3D)

add_executable(MVRE_Sample src/main.cpp)

add_subdirectory(src)

target_link_libraries(MVRE_Sample parallel)
target_link_libraries(MVRE_Sample MVRE)
target_link_libraries(MVRE_Sample MVRE_3D)
