cmake_minimum_required(VERSION 3.16)

project(monkey C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(monkey STATIC
    src/main.c
    src/rng.c
    src/crng.c
)

target_include_directories(monkey PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

if(MSVC)
    target_compile_options(monkey PRIVATE /W4)
else()
    target_compile_options(monkey PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()
