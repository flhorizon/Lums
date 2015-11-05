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

if (WIN32)
    # CMAKE_BUILD_TYPE is relevant ONLY in mono-configuration case (GNU Make output)
    # CMAKE_BUILD_TYPE is unused and empty in multi-configuration cases (VS .sln output)
    set(WIN_DEBUG_SUFFIX "d")
endif()

add_library(Lums_shared SHARED ${SRC} ${HEADER})
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME "Lums" DEFINE_SYMBOL "EXPORT_DLL")
set_target_properties(Lums_shared PROPERTIES OUTPUT_NAME_DEBUG "Lums${WIN_DEBUG_SUFFIX}" DEFINE_SYMBOL "EXPORT_DLL")
target_link_libraries(Lums_shared ${LINK_LIB})

if (WIN32 AND MSVC)

    # Copy Program DataBase files along DLLs, *if any*    
    add_custom_command(TARGET Lums_shared POST_BUILD
        COMMAND ${CMAKE_COMMAND} 
            -DSOURCE_FILE=$<TARGET_FILE_DIR:Lums_shared>/Lums$<$<CONFIG:Debug>:d>.pdb
            -DDESTINATION_FILE=${CMAKE_PDB_OUTPUT_DIRECTORY}/Lums$<$<CONFIG:Debug>:d>.pdb
            -P "${CMAKE_MODULE_PATH}/LmCopyIfExistsDifferent.cmake"
            DEPENDS $<TARGET_FILE_DIR:Lums_shared>/Lums$<$<CONFIG:Debug>:d>.pdb
    )
    
    # And install them. 
    install(FILES $<$<CONFIG:Debug>:${CMAKE_PDB_OUTPUT_DIRECTORY}/Lumsd.pdb> DESTINATION bin)
    # TODO: EXPORT PDBs and 3rd party DLLs
    
    # Copy and Install 3rd party DLLs and their PDBs
    list(APPEND tmp_Dep_Runtimes ${PNG_RUNTIME} ${ZLIB_RUNTIME} ${VORBISFILE_RUNTIME} ${VORBIS_RUNTIME} ${OGG_RUNTIME} ${FREETYPE_RUNTIME})  
    foreach(tmp_Dep_DLL_Src ${tmp_Dep_Runtimes})
        get_filename_component(tmp_DLL_Dir ${tmp_Dep_DLL_Src} DIRECTORY)
        get_filename_component(tmp_DLL_Basename ${tmp_Dep_DLL_Src} NAME)
        get_filename_component(tmp_DLL_Basename_WE ${tmp_DLL_Basename} NAME_WE)
        
        
        set(tmp_Runtime_Target "Lums_DllHell_${tmp_DLL_Basename_WE}")
        set(tmp_Dep_DLL_Dest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tmp_DLL_Basename}")
        set(tmp_Dep_PDB_Src "${tmp_DLL_Dir}/${tmp_DLL_Basename_WE}.pdb")
        set(tmp_Dep_PDB_Dest "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${tmp_DLL_Basename_WE}.pdb")

        if (NOT EXISTS ${tmp_Dep_PDB_Src})
            set(tmp_Effective_PDB_${tmp_DLL_Basename_WE})
        else()
            set(tmp_Effective_PDB_${tmp_DLL_Basename_WE} "${tmp_Dep_PDB_Src}")
        endif()
        
        
        add_custom_target(${tmp_Runtime_Target} ALL
            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${tmp_Dep_DLL_Src}" "${tmp_Dep_DLL_Dest}"
            COMMAND ${CMAKE_COMMAND} 
                -DSOURCE_FILE="${tmp_Dep_PDB_Src}"
                -DDESTINATION_FILE="${tmp_Dep_PDB_Dest}"
                -P "${CMAKE_MODULE_PATH}/LmCopyIfExistsDifferent.cmake"
            DEPENDS ${tmp_Dep_DLL_Src} ${tmp_Effective_PDB_${tmp_DLL_Basename_WE}}
        )        
        
        
        add_dependencies(Lums_shared ${tmp_Runtime_Target})        
        install(FILES ${tmp_Dep_DLL_Dest} ${tmp_Effective_PDB_${tmp_DLL_Basename_WE}} DESTINATION bin )        
               
    endforeach(tmp_Dep_DLL_Src)
    
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