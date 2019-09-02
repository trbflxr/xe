cmake_minimum_required(VERSION 2.8)
project(glad)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Ox")

find_package(OpenGL REQUIRED)

file(GLOB SRC
        src/glad.c
        src/glad_wgl.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(glad STATIC ${SRC})

target_link_libraries(glad ${OPENGL_LIBRARIES})
target_compile_options(glad PRIVATE "/w")
