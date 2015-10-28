#
# FindFreeType.cmake
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
    lm_helper_find_lib(FREETYPE_LIBRARIES NO_HINT ${WIN_LIB_PATH} freetype freetype26)
    lm_helper_find_runtimes(FREETYPE_RUNTIME NO_HINT ${WIN_LIB_PATH} freetype freetype26)
    
    find_path(FREETYPE_INCLUDE_DIRS ft2build.h
		PATH_SUFFIXES include
		PATHS
		${WIN_LIB_PATH}
	)
    
  
else()

	set(FREETYPE_SEARCH_PATH
		/usr
		/usr/local
		~/.brew
	)

	find_library(
		FREETYPE_LIBRARIES
		freetype
		PATHS
		${FREETYPE_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
		NO_DEFAULT_PATH
	)

	find_path(
	 	FREETYPE_INCLUDE_DIRS
	 	ft2build.h
	 	PATHS
	 	${FREETYPE_SEARCH_PATH}
	 	PATH_SUFFIXES include include/freetype2
	 	NO_DEFAULT_PATH
	)

endif()

message(STATUS "Found freetype: ${FREETYPE_LIBRARIES}")
