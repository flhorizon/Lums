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

	if (CMAKE_BUILD_TYPE MATCHES "Debug")
		set (WIN_DEBUG "d")
	endif()

# So lame... But it works.
	set (VORBIS_INCLUDE_DIRS "${WIN_LIB_PATH}/include")
	set (OGG_LIBRARIES "${WIN_LIB_PATH}/lib/libogg${WIN_DEBUG}.lib")
	set (OGG_RUNTIME "${WIN_LIB_PATH}/bin/libogg${WIN_DEBUG}.dll")
	set (VORBISFILE_LIBRARIES "${WIN_LIB_PATH}/lib/libvorbisfile${WIN_DEBUG}.lib")
	set (VORBISFILE_RUNTIME "${WIN_LIB_PATH}/bin/libvorbisfile${WIN_DEBUG}.dll")
	set (VORBIS_LIBRARIES "${WIN_LIB_PATH}/lib/libvorbis${WIN_DEBUG}.lib")
	set (VORBIS_RUNTIME "${WIN_LIB_PATH}/bin/libvorbis${WIN_DEBUG}.dll")
	
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
