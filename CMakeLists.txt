cmake_minimum_required(VERSION 3.0)
project(Lums)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CONFIGURATION_TYPES "Debug;Release")


if(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -std=c++11 -fno-exceptions -fno-rtti -Wall -Wno-multichar")
else()
    if (NOT EXISTS CMAKE_INSTALL_PREFIX)
        set(
            CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install"
            CACHE PATH "A default, unpriviledged install path on Windows with MSVC" FORCE
        )
    endif(NOT EXISTS CMAKE_INSTALL_PREFIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4244 /D_HAS_EXCEPTIONS=0 /GR- /vmg")
endif()

find_package(OpenGL REQUIRED)
find_package(OpenAL REQUIRED)
find_package(FreeType REQUIRED)
find_package(PNG REQUIRED)
find_package(Vorbis REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories(${PNG_INCLUDE_DIRS} SYSTEM)
include_directories(${FREETYPE_INCLUDE_DIRS} SYSTEM)
include_directories(${VORBIS_INCLUDE_DIRS} SYSTEM)

add_subdirectory(src)
add_subdirectory(util)
if (NOT WIN32)
	add_subdirectory(test)
endif()
add_subdirectory(example EXCLUDE_FROM_ALL)
