cmake_minimum_required(VERSION 3.1...3.27)

project(rayTracer VERSION 1.0.0 LANGUAGES CXX)

# Set the compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(rayTracer src/main.cpp)

target_compile_options(rayTracer PRIVATE
    -fdiagnostics-color=always
    -g
    -pedantic-errors
    -Wall
    -Weffc++
    -Wextra
    -Wsign-conversion
    -Wshadow
)

target_include_directories(rayTracer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)