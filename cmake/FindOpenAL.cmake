#
# FindOpenAL.cmake
#

if (WIN32)

	set(WIN_LIB_PATH ${CMAKE_SOURCE_DIR}/ext)
	if (MSVC)
		set(WIN_LIB_PATH "${WIN_LIB_PATH}/msvc")
	else()
		set(WIN_LIB_PATH "${WIN_LIB_PATH}/mingw")
	endif()
	if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
		set (WIN_LIB_PATH "${WIN_LIB_PATH}64")
	endif()

	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set (WIN_DEBUG "d")
	endif()

	set (OPENAL_LIBRARY "${WIN_LIB_PATH}/lib/OpenAL32${WIN_DEBUG}.lib")
	set (OPENAL_INCLUDE_DIRS "${WIN_LIB_PATH}/include/OpenAL")

else()

	find_path(OPENAL_INCLUDE_DIR al.h
		HINTS
		ENV OPENALDIR
		PATH_SUFFIXES include/AL include/OpenAL include
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/sw # Fink
		/opt/local # DarwinPorts
		/opt/csw # Blastwave
		/opt
	)
	
	find_library(OPENAL_LIBRARY
		NAMES OpenAL al openal OpenAL32
		HINTS
		ENV OPENALDIR
		PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
		PATHS
		~/Library/Frameworks
		/Library/Frameworks
		/sw
		/opt/local
		/opt/csw
		/opt
	)

endif()

message(STATUS "Found OpenAL: ${OPENAL_LIBRARIES}")
