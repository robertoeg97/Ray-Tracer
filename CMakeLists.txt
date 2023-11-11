cmake_minimum_required(VERSION 3.1...3.27)

project(ray-tracer VERSION 1.0.0 LANGUAGES CXX)

# Set the compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(ray-tracer 
    src/main.cpp
)
    
# Debug build flags
if (CMAKE_BUILD_TYPE STREQUAL "Debug")

    target_compile_options(ray-tracer PRIVATE
            -fdiagnostics-color=always
            -g
            -pedantic-errors
            -Wall
            -Wextra
            -Wsign-conversion
            -Wshadow
    )
#Release build flags  
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(ray-tracer PRIVATE
            -O3  # Use aggressive optimizations for Release
    )
endif ()

target_include_directories(ray-tracer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)