cmake_minimum_required(VERSION 3.1)
project(engine)

macro(add_msvc_pch pch_header pch_source sources)
  if (MSVC)
    GET_FILENAME_COMPONENT(PrecompiledBasename ${pch_header} NAME_WE)
    set(pch_bin "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledBasename}.pch")
    set(src ${${sources}})

    set_source_files_properties(${pch_source}
        PROPERTIES COMPILE_FLAGS "/Yc\"${pch_header}\" /Fp\"${pch_bin}\""
        OBJECT_OUTPUTS "${pch_bin}")
    set_source_files_properties(${src}
        PROPERTIES COMPILE_FLAGS "/Yu\"${pch_header}\" /FI\"${pch_header}\" /Fp\"${pch_bin}\""
        OBJECT_DEPENDS "${pch_bin}")
    # Add precompiled header to sources
    list(APPEND ${sources} ${pch_source})
  endif ()
endmacro()

set(CMAKE_CXX_STANDARD 17)

#options
option(XE_BUILD_SHARED_LIB "Build shared lib" ON)
option(XE_PLATFORM_WIN32 "Platform Win32" ON)
option(XE_PLATFORM_GL "Platform OpenGL" ON)
option(XE_USE_GENERIC_FS "Use generic filesystem" OFF)

set(XE_DEBUG_FLAGS "-Od -DXE_DEBUG -DXE_DEBUG_TRACING" CACHE STRING "xe_debug flags")
set(XE_RELEASE_FLAGS "-Ox -DNDEBUG" CACHE STRING "xe_release flags")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${XE_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${XE_RELEASE_FLAGS}")

set(XE_DEFINES
    -DXE_STATIC)

if (XE_PLATFORM_GL)
  set(XE_DEFINES "${XE_DEFINES} -DXE_PLATFORM_GL")
endif ()

if (XE_USE_GENERIC_FS)
  set(XE_DEFINES "${XE_DEFINES} -DXE_GENERIC_FS")
endif ()

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
add_subdirectory(external/glad)
include_directories(external/glad/include)

add_subdirectory(external/glfw)
include_directories(external/glfw/include)

add_subdirectory(external/tbb)
include_directories(external/tbb/include)

add_subdirectory(external/spdlog)
include_directories(external/spdlog/include)

#includes
include_directories(${XE_INCLUDE_DIRS})
include_directories(src)

#engine sources
file(GLOB ENGINE_SRC
    src/*.cpp
    src/core/*.cpp
    src/external/minitrace/*.cpp
    src/graphics/*.cpp
    src/math/*.cpp
    src/utils/*.cpp)

#win32 sources
file(GLOB ENGINE_WIN32_SRC
    src/core/win32/*.cpp)

#opengl sources
file(GLOB ENGINE_GL_SRC
    src/graphics/opengl/*.cpp)

if (XE_PLATFORM_WIN32)
  list(APPEND ENGINE_SRC ${ENGINE_WIN32_SRC})
endif ()

if (XE_PLATFORM_GL)
  list(APPEND ENGINE_SRC ${ENGINE_GL_SRC})
endif ()

#add pch
add_msvc_pch("xepch.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/xepch/xepch.cpp" ENGINE_SRC)

#xe lib
if (XE_BUILD_SHARED_LIB)
  add_library(xe SHARED ${ENGINE_SRC})
else ()
  add_library(xe STATIC ${ENGINE_SRC})
endif ()

target_compile_definitions(xe PUBLIC ${XE_DEFINES})

target_link_libraries(xe
    tbb_static
    spdlog)

#link opengl libs
if (XE_PLATFORM_GL)
  target_link_libraries(xe
      glad
      glfw)
endif ()

#link win32 libs
if (XE_PLATFORM_WIN32)
  target_link_libraries(xe winmm)
endif ()
