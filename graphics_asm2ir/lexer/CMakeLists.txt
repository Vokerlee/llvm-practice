cmake_minimum_required(VERSION 3.13)

set(SOURCES lexer.cpp)

add_library(lexer_impl OBJECT ${SOURCES})
target_include_directories(lexer_impl PUBLIC ${GRASM_ROOT})