cmake_minimum_required(VERSION 3.13)

set(SOURCES ast/control_flow.cpp)

add_library(llvmgen_impl OBJECT ${SOURCES})
target_include_directories(llvmgen_impl PUBLIC ${GRLANG_ROOT})
