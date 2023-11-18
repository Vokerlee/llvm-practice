cmake_minimum_required(VERSION 3.13)

set(SOURCES CPU.cpp instruction_exec.cpp)

add_library(cpu_impl OBJECT ${SOURCES})
target_include_directories(cpu_impl PUBLIC ${GRASM_ROOT} ${SGL_INCLUDE_DIRS})
