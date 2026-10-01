cmake_minimum_required(VERSION 3.20)
project(matmul_lab CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O3 -march=native)
endif()

add_executable(matmul_benchmark
        main.cpp
        blocks_matmul.h
        matrix.h
)