cmake_minimum_required(VERSION 3.13)

bison_target(parser grammar.y ${GRLANG_BINARY_ROOT}/parser.cpp
             DEFINES_FILE ${GRLANG_BINARY_ROOT}/parser.h)

message(STATUS "Bison output: ${BISON_parser_OUTPUTS}")

flex_target(lexer lexer.lex ${GRLANG_BINARY_ROOT}/lexer.cpp)
message(STATUS "Flex output: ${FLEX_lexer_OUTPUTS}")

add_flex_bison_dependency(lexer parser)

set(SOURCES
    ${GRLANG_BINARY_ROOT}/parser.cpp
    ${GRLANG_BINARY_ROOT}/lexer.cpp
)

add_library(grammar_impl OBJECT ${SOURCES})
target_include_directories(grammar_impl PUBLIC ${GRLANG_ROOT})
