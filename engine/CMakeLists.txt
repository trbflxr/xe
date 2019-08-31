cmake_minimum_required(VERSION 3.1)
project(engine)

set(CMAKE_CXX_STANDARD 17)

#options
option(XE_BUILD_SHARED_LIB "Build shared lib" ON)
option(XE_PLATFORM_WIN32 "Platform Win32" ON)
option(XE_PLATFORM_GL "Platform OpenGL" ON)

set(XE_DEBUG_FLAGS "-DXE_DEBUG -Wall" CACHE STRING "xe_debug defines, compile options")
set(XE_RELEASE_FLAGS "-Ofast -DNDEBUG" CACHE STRING "xe_release defines, compile options")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${XE_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${XE_RELEASE_FLAGS}")

set(XE_DEFINES
        -DXE_STATIC)

#status
message(STATUS "XE Configuration:")
message(STATUS "  XE_BUILD_SHARED_LIB:     " ${XE_BUILD_SHARED_LIB})
message(STATUS "  XE_PLATFORM_WIN32:       " ${XE_PLATFORM_WIN32})
message(STATUS "  XE_PLATFORM_GL:          " ${XE_PLATFORM_GL})
message(STATUS "  XE_USE_GENERIC_FS:       " ${XE_USE_GENERIC_FS})
message(STATUS "  XE_DEFINES:              " ${XE_DEFINES})
message(STATUS "  CMAKE_CXX_FLAGS_DEBUG:   " ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "  CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE})

set(XE_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        CACHE STRING "XE include dirs" FORCE)

#external libs
add_subdirectory(external/fmt)
include_directories(external/fmt/include)

add_subdirectory(external/glad)
include_directories(external/glad/include)

#includes
include_directories(${XE_INCLUDE_DIRS})
include_directories(src)

#engine sources
file(GLOB ENGINE_SRC
        src/*.cpp)

#win32 sources
file(GLOB ENGINE_WIN32_SRC)

#opengl sources
file(GLOB ENGINE_GL_SRC)

#total platform files
file(GLOB PLATFORM_SRC)

if (XE_PLATFORM_WIN32)
    list(APPEND PLATFORM_SRC ${ENGINE_WIN32_SRC})
endif ()

if (XE_PLATFORM_GL)
    list(APPEND PLATFORM_SRC ${ENGINE_GL_SRC})
endif ()

#xe lib
if (XE_BUILD_SHARED_LIB)
    add_library(xe SHARED ${PLATFORM_SRC} ${ENGINE_SRC})
else ()
    add_library(xe STATIC ${PLATFORM_SRC} ${ENGINE_SRC})
endif ()

target_compile_definitions(xe PUBLIC ${XE_DEFINES})

target_link_libraries(xe
        fmt)

#link opengl libs
if (XE_PLATFORM_GL)
    target_link_libraries(xe glad)
endif ()

#link win32 libs
if (XE_PLATFORM_WIN32)
    target_link_libraries(xe winmm)
endif ()
