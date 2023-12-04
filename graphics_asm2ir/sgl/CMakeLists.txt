cmake_minimum_required(VERSION 3.13)

set(SOURCES sgl.cpp)

add_library(sgl_impl OBJECT ${SOURCES})
target_include_directories(sgl_impl PUBLIC ${GRASM_ROOT})