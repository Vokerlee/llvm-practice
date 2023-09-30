cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(GraphicsApp VERSION 1.0 LANGUAGES C)

set(GR_APP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(GR_APP_ROOT_BINARY_ROOT ${CMAKE_CURRENT_BINARY_DIR})

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wall -Wextra -Werror -Wshadow")

add_library(sgl sgl/sgl.c)
target_link_libraries     (sgl PRIVATE ${GTK_LIBRARIES})
target_include_directories(sgl PRIVATE ${GTK_INCLUDE_DIRS})

add_executable(graph_app main.c gameoflife.c)
target_link_libraries(graph_app PUBLIC sgl)
