cmake_minimum_required(VERSION 3.1)
project(editor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-static-libgcc -static-libstdc++")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${XE_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${XE_RELEASE_FLAGS}")

include_directories(${XE_INCLUDE_DIRS})
include_directories(src)

#add_definitions()

#sandbox
file(GLOB SRC
        src/*.cpp)

add_executable(editor ${SRC})

target_link_libraries(editor xe)

file(COPY assets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
