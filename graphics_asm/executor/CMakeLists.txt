cmake_minimum_required(VERSION 3.13)

set(SOURCES executor.cpp)

add_library(executor_impl OBJECT ${SOURCES})
target_include_directories(executor_impl PUBLIC ${GRASM_ROOT})