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

#if (MSVC OR WIN32 AND CMAKE_BUILD_TYPE MATCHES "Debug")
#    # CMAKE_BUILD_TYPE is relevant ONLY in mono-configuration case (GNU Make output)
#    # CMAKE_BUILD_TYPE is unused and empty in multi-configuration cases (VS .sln output)
#    set (WIN_DEBUG "d")
#endif()

add_library(Lums_shared SHARED ${SRC} ${HEADER})
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME "Lums" DEFINE_SYMBOL "EXPORT_DLL")
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME_DEBUG "Lumsd" DEFINE_SYMBOL "EXPORT_DLL")
target_link_libraries(Lums_shared ${LINK_LIB})

if (WIN32 AND MSVC)

    # Sane warning; shouldn't disable it and fix the causes
    #get_target_properties(tmp_Lums_shared_LNK  Lums_shared LINK_FLAGS_DEBUG)
    #set_target_properties(Lums_shared PROPERTIES LINK_FLAGS_DEBUG "${tmp_Lums_shared_LNK} /NODEFAULTLIB:MSVCRT")
    #unset(tmp_Lums_shared_LNK)

    # Ease solving the DLL hell for the client software.
    #set(LUMS_RUNTIME_DEPS ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME})
    
    # Get rid of those annoying <buildtype> subdirectories !
    # CMAKE_*_OUTPUT_DIRECTORY are set in the project top-level CMakeLists.txt
    # get_target_property(tmp_RT_OUTPUT_DIR Lums_shared RUNTIME_OUTPUT_DIRECTORY)
    # get_target_property(tmp_LIB_OUTPUT_DIR Lums_shared LIBRARY_OUTPUT_DIRECTORY)
    # get_target_property(tmp_AR_OUTPUT_DIR Lums_shared ARCHIVE_OUTPUT_DIRECTORY)
    # foreach( tmp_OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
       # string( TOUPPER ${tmp_OUTPUTCONFIG} tmp_OUTPUTCONFIG )        
        
       # set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_RT_OUTPUT_DIR}" )
       # #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_RUNTIME_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_RT_OUTPUT_DIR}")
        
       # set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_LIB_OUTPUT_DIR}" )
       # #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_LIBRARY_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_LIB_OUTPUT_DIR}")
       
       # set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} "${tmp_AR_OUTPUT_DIR}" )
       # #message(STATUS "CMAKE INSTALL DEBUG: CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${tmp_OUTPUTCONFIG} == ${tmp_AR_OUTPUT_DIR}")
        
        
    # endforeach( tmp_OUTPUTCONFIG )
    
    # FIXME: Handle this the multi-configuration way
    list(APPEND tmp_Dep_Runtimes ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME} )  
    foreach(tmp_Dep_DLL_Src ${tmp_Dep_Runtimes})
        get_filename_component(tmp_DLL_Dir ${tmp_Dep_DLL_Src} DIRECTORY)
        get_filename_component(tmp_DLL_Basename ${tmp_Dep_DLL_Src} NAME)
        get_filename_component(tmp_DLL_Basename_WE ${tmp_DLL_Basename} NAME_WE)
      
      
        
        set(tmp_Runtime_Target "Lums_DllHell_${tmp_DLL_Basename_WE}")
        set(tmp_Dep_DLL_Src_Dest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tmp_DLL_Basename}")
        set(tmp_Dep_PDB_Src "${tmp_DLL_Dir}/${tmp_DLL_Basename_WE}.pdb")
        set(tmp_Dep_PDB_Dest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tmp_DLL_Basename_WE}.pdb")
        
        message(STATUS "--DLLHELL - cp target output :  ${tmp_Dep_DLL_Src_Dest}")
        
        add_custom_target(${tmp_Runtime_Target} ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${tmp_Dep_PDB_Src}" "${tmp_Dep_PDB_Dest}"
            DEPENDS "${tmp_Dep_DLL_Src_Dest}"
        )
        
        add_custom_command(TARGETS Lums_shared POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different  "${tmp_Dep_PDB_Src}" "${tmp_Dep_PDB_Dest}"
        )
        
        add_dependencies(Lums_shared ${tmp_Runtime_Target})        
        install(FILES "${tmp_Dep_DLL_Src_Dest};${tmp_Dep_PDB_Dest}" DESTINATION bin )
        
        #add_custom_command(
        #    OUTPUT ${tmp_Dep_DLL_Src_Dest}
        #    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${tmp_Dep_DLL_Src}" "${tmp_Dep_DLL_Src_Dest}"
        #)
        #add_custom_target(${tmp_Runtime_Target} ALL DEPENDS "${tmp_Dep_DLL_Src_Dest}")
        #install(FILES "${tmp_Dep_DLL_Src_Dest}" DESTINATION bin)        
        #export(TARGETS ${tmp_Runtime_Target} FILE lib/cmake/DLLHell-export.cmake)
        
    endforeach(tmp_Dep_DLL_Src)
    
    
    
    
    ## Original copy instruction....
    # file(
        # COPY
        # ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME}
        # DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    # )
    
    unset(tmp_Dep_Runtimes)
    unset(tmp_Dep_DLL_Src)
    unset(tmp_DLL_Basename)
    unset(tmp_DLL_Basename_WE)
    unset(tmp_Runtime_Target)
    unset(tmp_Dep_DLL_Src_Dest)
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
endif()

# Also create an export file and perform a buildtree install to ease discovery from client projects

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/LumsInclude DESTINATION include)
install(FILES ${CMAKE_SOURCE_DIR}/include/Lums DESTINATION include)
add_custom_target(Lums DEPENDS ${LUMS_ALL})