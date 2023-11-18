cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(GRASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GRASM_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GRASM_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -fno-exceptions")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GRASM_BINARY_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GRASM_BINARY_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GRASM_BINARY_ROOT}/bin)

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

add_subdirectory(parser)

add_library(grasm_static STATIC)
target_link_libraries(grasm_static PUBLIC
    parser_impl
)

# ----------------------------------------------------------------------------

add_executable(grasm main.cpp)
target_include_directories(grasm PUBLIC ${GRASM_ROOT} ${LLVM_INCLUDE_DIRS})
target_link_libraries(grasm PUBLIC grasm_static ${llvm_libs})
