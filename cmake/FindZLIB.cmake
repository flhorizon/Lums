#
# FindZLIB.cmake
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
    lm_helper_find_lib(ZLIB_LIBRARIES NO_HINT ${WIN_LIB_PATH} zlib)
    lm_helper_find_runtimes(ZLIB_RUNTIME NO_HINT ${WIN_LIB_PATH} zlib)

    find_path(ZLIB_INCLUDE_DIRS zlib.h
		PATH_SUFFIXES include
		PATHS ${WIN_LIB_PATH}
	)
    
else()

	set(ZLIB_SEARCH_PATH
		/usr
		/usr/local
		~/.brew
	)

	find_library(
		ZLIB_LIBRARIES
		z
		PATHS
		${ZLIB_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
	)

	find_path(
	 	ZLIB_INCLUDE_DIRS
	 	zlib.h
	 	PATHS
	 	${ZLIB_SEARCH_PATH}
	 	PATH_SUFFIXES include
	 	NO_DEFAULT_PATH
	)

endif()


message(STATUS "Found zlib: ${ZLIB_LIBRARIES}")
