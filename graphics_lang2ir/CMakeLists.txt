cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(GRLANG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(GRLANG_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GRLANG_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${GRLANG_BINARY_ROOT}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${GRLANG_BINARY_ROOT}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GRLANG_BINARY_ROOT}/bin)

find_package(SDL2 REQUIRED)
find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

find_package(BISON)
find_package(FLEX)

add_subdirectory(llvmgen)
add_subdirectory(grammar)
add_subdirectory(stdlib)

add_library(grlang_static STATIC)
target_link_libraries(grlang_static PUBLIC
    llvmgen_impl
    grammar_impl
)

# ----------------------------------------------------------------------------

add_executable(grlang main.cpp)
target_include_directories(grlang PUBLIC ${GRLANG_ROOT} ${LLVM_INCLUDE_DIRS})
target_link_libraries(grlang PUBLIC grlang_static LLVM ${SDL2_LIBRARIES})
