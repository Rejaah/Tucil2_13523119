cmake_minimum_required(VERSION 3.10)
project(Tucil2_1352119)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(src)
include_directories(src/utils)
file(GLOB_RECURSE SOURCES src/*.cpp)

add_executable(Tucil2_1352119 ${SOURCES})