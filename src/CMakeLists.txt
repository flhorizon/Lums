file(GLOB SRC "*.cpp" "Binary/*.cpp" "Graphics/*.cpp" "Graphics/ImageFormat/*.cpp" "Audio/*.cpp" "Core/*.cpp" "GameObject/*.cpp" "Skeleton/*.cpp")
file(GLOB_RECURSE HEADER "../include/LumsInclude/**.hpp" "../include/Lums")
set(LINK_LIB ${OPENGL_gl_LIBRARY} ${OPENAL_LIBRARY} ${PNG_LIBRARIES} ${FREETYPE_LIBRARIES} ${VORBIS_LIBRARIES} )

if(APPLE)
    file(GLOB OS_SRC "OS/MacOSX/*.cpp" "OS/MacOSX/*.mm")
    include_directories("OS/MacOSX")
    list(APPEND LINK_LIB "-framework Cocoa" "-framework IOKit")
endif()

if(WIN32)
	file(GLOB OS_SRC "OS/WinNT/*.cpp" "OS/WinNT/*.c")
	include_directories("OS/WinNT")
endif()

list(APPEND SRC ${OS_SRC})

set(LUMS_ALL lums_shared)

if (MSVC OR WIN32 AND CMAKE_BUILD_TYPE MATCHES "Debug")
    # CMAKE_BUILD_TYPE is relevant ONLY in mono-configuration case (GNU Make output)
    # CMAKE_BUILD_TYPE is unused and empty in multi-configuration cases (VS .sln output)
    set (WIN_DEBUG "d")
endif()

add_library(Lums_shared SHARED ${SRC} ${HEADER})
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME "Lums" DEFINE_SYMBOL "EXPORT_DLL")
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME_DEBUG "Lums${WIN_DEBUG}" DEFINE_SYMBOL "EXPORT_DLL")
target_link_libraries(Lums_shared ${LINK_LIB})

if (WIN32 AND MSVC)

    # Ease solving the DLL hell for the client software.
    #set(LUMS_RUNTIME_DEPS ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME})
    
    # Get rid of those annoying <buildtype> subdirectories !
    # CMAKE_*_OUTPUT_DIRECTORY are set in the project top-level CMakeLists.txt
    get_target_property(tmp_RT_OUTPUT_DIR Lums_shared RUNTIME_OUTPUT_DIRECTORY)
    get_target_property(tmp_LIB_OUTPUT_DIR Lums_shared LIBRARY_OUTPUT_DIRECTORY)
    get_target_property(tmp_AR_OUTPUT_DIR Lums_shared ARCHIVE_OUTPUT_DIRECTORY)
    foreach( tmp_OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       string( TOUPPER ${tmp_OUTPUTCONFIG} tmp_OUTPUTCONFIG )        
        
       set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_RT_OUTPUT_DIR}" )
       #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_RUNTIME_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_RT_OUTPUT_DIR}")
        
       set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_LIB_OUTPUT_DIR}" )
       #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_LIBRARY_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_LIB_OUTPUT_DIR}")
       
       set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_AR_OUTPUT_DIR}" )
       #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_AR_OUTPUT_DIR}")
        
        
    endforeach( tmp_OUTPUTCONFIG )
    
    # FIXME: Handle this the multi-configuration way
    list(APPEND tmp_RT_DEPENDENCIES ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME} )  
    foreach(tmp_EXTDLL ${tmp_RT_DEPENDENCIES})
        get_filename_component(tmp_RT_Name ${tmp_EXTDLL} NAME)
        get_filename_component(tmp_DLL_TGT ${tmp_RT_Name} NAME_WE)
      
      
        
        set(tmp_RT_EMBED_TARGET "DllHell_${tmp_DLL_TGT}")
        set(tmp_RT_EMBED_DEST_FILE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tmp_RT_Name}")
        
        #message(STATUS "--DLLHELL - cp target output :  ${tmp_RT_EMBED_DEST_FILE}")
        
        add_custom_target(${tmp_RT_EMBED_TARGET} ALL
            ${CMAKE_COMMAND} -E copy_if_different "${tmp_EXTDLL}" "${tmp_RT_EMBED_DEST_FILE}"
            DEPENDS "${tmp_RT_EMBED_DEST_FILE}"
        )
        
        add_dependencies(Lums_shared ${tmp_RT_EMBED_TARGET})        
        install(FILES "${tmp_RT_EMBED_DEST_FILE}" DESTINATION bin )
        
        #add_custom_command(
        #    OUTPUT ${tmp_RT_EMBED_DEST_FILE}
        #    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${tmp_EXTDLL}" "${tmp_RT_EMBED_DEST_FILE}"
        #)
        #add_custom_target(${tmp_RT_EMBED_TARGET} ALL DEPENDS "${tmp_RT_EMBED_DEST_FILE}")
        #install(FILES "${tmp_RT_EMBED_DEST_FILE}" DESTINATION bin)        
        #export(TARGETS ${tmp_RT_EMBED_TARGET} FILE lib/cmake/DLLHell-export.cmake)
        
    endforeach(tmp_EXTDLL)
    
    
    
    
   
    # file(
        # COPY
        # ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME}
        # DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    # )
    unset(tmp_RT_DEPENDENCIES)
    unset(tmp_RT_OUTPUT_DIR)
    unset(tmp_LIB_OUTPUT_DIR)
    unset(tmp_AR_OUTPUT_DIR)
endif()

install(TARGETS Lums_shared EXPORT Lums_shared-export
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
install(EXPORT Lums_shared-export DESTINATION lib/cmake)

if (NOT WIN32)
	add_library(Lums_static STATIC ${SRC} ${HEADER})
	set_target_properties(Lums_static PROPERTIES OUTPUT_NAME Lums)
	install(TARGETS Lums_static DESTINATION lib)
	list(APPEND LUMS_ALL lums_static)
endif()

if (APPLE)
    add_library(Lums_framework SHARED ${SRC} ${HEADER})
    set_target_properties(Lums_framework PROPERTIES OUTPUT_NAME Lums FRAMEWORK ON PUBLIC_HEADER "${HEADER}")
    target_link_libraries(Lums_framework ${LINK_LIB})
    install(TARGETS Lums_framework DESTINATION Library/Frameworks)
    list(APPEND LUMS_ALL lums_framework)
endif ()

# Also create an export file and perform a buildtree install to ease discovery from client projects

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/LumsInclude DESTINATION include)
install(FILES ${CMAKE_SOURCE_DIR}/include/Lums DESTINATION include)
add_custom_target(Lums DEPENDS ${LUMS_ALL})