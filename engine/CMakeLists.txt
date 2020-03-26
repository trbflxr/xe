cmake_minimum_required(VERSION 3.1)
project(engine)

set(CMAKE_CXX_STANDARD 17)

#pch
include(cmake/msvc_pch.cmake)

#target
set(ENGINE_NAME xe)

#options
option(XE_PLATFORM_WIN32 "Platform Win32" ON)
option(XE_PLATFORM_GL "Platform OpenGL" ON)
option(XE_THREAT_WARNINGS_AS_ERRORS "Threat warnings as errors" OFF)
option(XE_ENABLE_DEBUG_TRACING "Enable debug tracing for chrome://tracing" ON)

set(XE_DEBUG_FLAGS "-Od -DXE_DEBUG" CACHE STRING "xe_debug flags")
set(XE_RELEASE_FLAGS "-Ox -DNDEBUG" CACHE STRING "xe_release flags")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${XE_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${XE_RELEASE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} ${XE_DEBUG_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} ${XE_RELEASE_FLAGS}")

#some libs requires debug versions on msvc
set(XE_DEBUG_SUFFIX "")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(XE_DEBUG_SUFFIX "d")
endif ()

if (XE_THREAT_WARNINGS_AS_ERRORS)
  set(XE_WARNINGS "/W4 /WX")
else ()
  set(XE_WARNINGS "/W4")
endif ()

set(XE_DEFINES
    -DXE_STATIC
    -DXE_CURRENT_SRC_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}\")

set(XE_EXT_LIBS
    spdlog
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/glfw3${XE_DEBUG_SUFFIX}.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/external/physfs/physfs${XE_DEBUG_SUFFIX}.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/external/msdfgen/msdfgen${XE_DEBUG_SUFFIX}.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/external/msdfgen/msdfatlasgen${XE_DEBUG_SUFFIX}.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/external/msdfgen/freetype.lib)

if (XE_PLATFORM_WIN32)
  list(APPEND XE_DEFINES -DXE_PLATFORM_WINDOWS)
  list(APPEND XE_EXT_LIBS winmm)
endif ()

if (XE_PLATFORM_GL)
  list(APPEND XE_DEFINES -DXE_PLATFORM_GL)
endif ()

if (XE_ENABLE_DEBUG_TRACING)
  list(APPEND XE_DEFINES -DXE_DEBUG_TRACING)
endif ()

#status
message(STATUS "XE Configuration:")
message(STATUS "  XE_WARNINGS:             " ${XE_WARNINGS})
message(STATUS "  XE_PLATFORM_WIN32:       " ${XE_PLATFORM_WIN32})
message(STATUS "  XE_PLATFORM_GL:          " ${XE_PLATFORM_GL})
message(STATUS "  XE_ENABLE_DEBUG_TRACING: " ${XE_ENABLE_DEBUG_TRACING})
message(STATUS "  XE_DEFINES:              " ${XE_DEFINES})
message(STATUS "  XE_EXT_LIBS:             " ${XE_EXT_LIBS})
message(STATUS "  CMAKE_CXX_FLAGS_DEBUG:   " ${CMAKE_CXX_FLAGS_DEBUG})
message(STATUS "  CMAKE_CXX_FLAGS_RELEASE: " ${CMAKE_CXX_FLAGS_RELEASE})

set(XE_INCLUDE_DIRS
    #engine
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    #libs
    ${CMAKE_CURRENT_SOURCE_DIR}/external/entt/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/spdlog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/physfs/include
    ${CMAKE_CURRENT_SOURCE_DIR}/external/msdfgen/include

    CACHE STRING "XE include dirs" FORCE)

#includes
include_directories(${XE_INCLUDE_DIRS})
include_directories(src)

#submodules
add_subdirectory(external/spdlog)

#engine sources
file(GLOB ENGINE_EXTERNAL_SRC
    src/external/glad/*.c
    src/external/minitrace/*.c)

file(GLOB ENGINE_SRC
    src/*.cpp
    src/components/*.cpp
    src/core/*.cpp
    src/embedded/*.cpp
    src/graphics/*.cpp
    src/math/*.cpp
    src/serialization/*.cpp
    src/ui/*.cpp
    src/ui/imgui/*.cpp
    src/utils/*.cpp)

#win32 sources
file(GLOB ENGINE_WIN32_SRC
    src/core/win32/*.cpp)

#opengl sources
file(GLOB ENGINE_GL_SRC
    src/graphics/opengl/*.cpp
    src/ui/opengl/*.cpp)

#append platform sources
if (XE_PLATFORM_WIN32)
  list(APPEND ENGINE_SRC ${ENGINE_WIN32_SRC})
endif ()

if (XE_PLATFORM_GL)
  list(APPEND ENGINE_SRC ${ENGINE_GL_SRC})
endif ()

#add pch
add_msvc_pch("xepch.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/xepch/xepch.cpp" ENGINE_SRC)

#append ext libs src
list(APPEND ENGINE_SRC ${ENGINE_EXTERNAL_SRC})

#xe lib
add_library(${ENGINE_NAME} STATIC ${ENGINE_SRC})

target_compile_definitions(${ENGINE_NAME} PUBLIC ${XE_DEFINES})

target_link_libraries(${ENGINE_NAME} ${XE_EXT_LIBS})

#replace /w3 by /w4
if (MSVC)
  if (CMAKE_CXX_FLAGS MATCHES "/W[0-3]")
    string(REGEX REPLACE "/W[0-3]" ${XE_WARNINGS} CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${XE_WARNINGS}")
  endif ()
endif ()

#pack shaders
execute_process(COMMAND ${CMAKE_COMMAND}
    "-DSHADERS_DIR=${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/opengl/shaders"
    "-DOUT_DIR=${CMAKE_CURRENT_SOURCE_DIR}/src/graphics"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/shader_packer.cmake")
