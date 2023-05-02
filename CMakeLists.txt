cmake_minimum_required(VERSION 3.25)
project(MMine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23")

add_subdirectory(MARS)
add_subdirectory(MARS_3D)
add_subdirectory(MPE)
add_subdirectory(MFR)

add_executable(MMine src/main.cpp)

add_subdirectory(src)

target_link_libraries(MMine MARS)