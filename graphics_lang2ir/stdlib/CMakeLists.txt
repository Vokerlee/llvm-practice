cmake_minimum_required(VERSION 3.13)

set(SOURCES
    io.cpp
    sgl.cpp
)

add_library(stdlib_impl OBJECT ${SOURCES})
target_include_directories(stdlib_impl PUBLIC ${GRLANG_ROOT})
