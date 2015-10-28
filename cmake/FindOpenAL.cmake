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

    
    include(${CMAKE_MODULE_PATH}/LmHelperFindLib.cmake)
    lm_helper_find_lib(OPENAL_LIBRARY OPENALDIR ${WIN_LIB_PATH} OpenAL al openal OpenAL32)
    lm_helper_find_runtimes(OPENAL_RUNTIME OPENALDIR ${WIN_LIB_PATH} OpenAL al openal OpenAL32)
    
       
    find_path(OPENAL_INCLUDE_DIR al.h
		HINTS
		ENV OPENALDIR
		PATH_SUFFIXES include/AL include/OpenAL include
		PATHS
		${WIN_LIB_PATH}
	)
    
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

message(STATUS "Found OpenAL: ${OPENAL_LIBRARY}")

