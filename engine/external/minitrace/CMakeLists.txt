cmake_minimum_required(VERSION 2.8)
project(minitrace)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Ofast")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/minitrace)

file(GLOB SRC
        minitrace.cpp)

add_library(minitrace STATIC ${SRC})
target_compile_options(minitrace PRIVATE "/w")
