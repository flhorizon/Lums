#
# FindVorbis.cmake
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
    
    lm_helper_find_lib(VORBIS_LIBRARIES NO_HINT ${WIN_LIB_PATH} libvorbis)
    lm_helper_find_lib(OGG_LIBRARIES NO_HINT ${WIN_LIB_PATH} libogg)
    lm_helper_find_lib(VORBISFILE_LIBRARIES NO_HINT ${WIN_LIB_PATH} libvorbisfile)
    lm_helper_find_runtimes(VORBIS_RUNTIME NO_HINT ${WIN_LIB_PATH} libogg libvorbis libvorbisfile)
    
    
    find_path(VORBIS_INCLUDE_DIRS vorbisfile.h
		PATH_SUFFIXES include include/vorbis include/ogg
		PATHS
		${WIN_LIB_PATH}
	)
    
else()

	set(VORBIS_SEARCH_PATH
		/usr
		/usr/local
		~/.brew
	)
	
	find_library(
		VORBIS_LIBRARIES
		vorbis
		PATHS
		${VORBIS_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
	)
	
	find_library(
		OGG_LIBRARIES
		ogg
		PATHS
		${VORBIS_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
	)
	
	find_library(
		VORBISFILE_LIBRARIES
		vorbisfile
		PATHS
		${VORBIS_SEARCH_PATH}
		PATH_SUFFIXES lib lib64
	)
	
	find_path(
		VORBIS_INCLUDE_DIRS
		vorbis/vorbisfile.h
		PATHS
		${VORBIS_SEARCH_PATH}
		PATH_SUFFIXES include
		NO_DEFAULT_PATH
	)
	
endif()

list(APPEND VORBIS_LIBRARIES ${OGG_LIBRARIES} ${VORBISFILE_LIBRARIES})
list(APPEND VORBIS_INCLUDE_DIRS ${OGG_INCLUDE_DIRS})

message(STATUS "Found vorbis: ${VORBIS_LIBRARIES}")
