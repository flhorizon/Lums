
# LmHelperFindLib.cmake

# OUTPUT_LIBRARIES: parent scope variable name to set with found libs list
# ENV_HINT: Environment variable name providing a hint. Use NO_HINT to avoir prviding one.
# LITTERAL_PATHS: An expanded list of hard coded paths to walk. Mandatory.
# more args: the base names to look for.

function(lm_helper_find_lib OUTPUT_LIBRARIES ENV_HINT LITTERAL_PATHS)
    list(REMOVE_AT ARGV 2 1 0)
    set(LIB_NAMES ${ARGV})
    set(DEBUG_LIB_NAMES ${ARGV})
    set(FOUND_SO_FAR)
    
    set(DEBUG_LIB_NAMES ${LIB_NAMES})
    set(WIN_DEBUG_SUFFIX "d")
    foreach(A_NAME IN LISTS DEBUG_LIB_NAMES)
        list(INSERT LIB_NAMES 0 "${A_NAME}${WIN_DEBUG_SUFFIX}")
    endforeach(A_NAME)
    
    list(LENGTH LIB_NAMES NAMES_LEN)
    while (NAMES_LEN GREATER 0)
        list(GET LIB_NAMES 0 A_NAME)
        find_library(OUTVAR_${A_NAME}
            NAMES ${A_NAME}
            HINTS ENV ${ENV_HINT}
            PATH_SUFFIXES lib64 lib libs64 libs libs/Win32 libs/Win64
            PATHS
            ${LITTERAL_PATHS}
        )
        if (NOT OUTVAR_${A_NAME} MATCHES "OUTVAR_${A_NAME}-NOTFOUND")
            list(APPEND FOUND_SO_FAR ${OUTVAR_${A_NAME}})
        endif()
        
        list(REMOVE_AT LIB_NAMES 0)
        list(LENGTH LIB_NAMES NAMES_LEN)
    endwhile(NAMES_LEN GREATER 0)
    
    list(LENGTH FOUND_SO_FAR RESULT_LEN)
    if (RESULT_LEN LESS 1)
        set(${OUTPUT_LIBRARIES} "${OUTPUT_LIBRARIES}-NOTFOUND" PARENT_SCOPE)
    else()
        set(${OUTPUT_LIBRARIES} ${FOUND_SO_FAR} PARENT_SCOPE)
    endif()
endfunction(lm_helper_find_lib)




function(lm_helper_find_runtimes OUTPUT_RUNTIMES ENV_HINT LITTERAL_PATHS)
    list(REMOVE_AT ARGV 2 1 0)
    set(DLL_NAMES ${ARGV})
    set(FOUND_SO_FAR)
    
    set(ALL_DLL_NAMES)
    set(WIN_DEBUG_SUFFIX "d")
    foreach(A_NAME IN LISTS DLL_NAMES)
        list(INSERT ALL_DLL_NAMES 0 "${A_NAME}${WIN_DEBUG_SUFFIX}")
        list(INSERT ALL_DLL_NAMES 0 "${A_NAME}${WIN_DEBUG_SUFFIX}.dll" "${A_NAME}.dll")
    endforeach(A_NAME)
    
    list(LENGTH ALL_DLL_NAMES NAMES_LEN)
    while (NAMES_LEN GREATER 0)
        list(GET ALL_DLL_NAMES 0 A_NAME)
        find_file(OUTVAR_${A_NAME}
            NAMES ${A_NAME}
            HINTS ENV ${ENV_HINT}
            PATH_SUFFIXES bin
            PATHS
            ${LITTERAL_PATHS}
        )
        if (NOT OUTVAR_${A_NAME} MATCHES "OUTVAR_${A_NAME}-NOTFOUND")
            list(APPEND FOUND_SO_FAR ${OUTVAR_${A_NAME}})
        endif()
        
        list(REMOVE_AT ALL_DLL_NAMES 0)
        list(LENGTH ALL_DLL_NAMES NAMES_LEN)
    endwhile(NAMES_LEN GREATER 0)
    
    list(LENGTH FOUND_SO_FAR RESULT_LEN)
    if (RESULT_LEN LESS 1)
        set(${OUTPUT_RUNTIMES} "${OUTPUT_RUNTIMES}-NOTFOUND" PARENT_SCOPE)
    else()
        set(${OUTPUT_RUNTIMES} ${FOUND_SO_FAR} PARENT_SCOPE)
    endif()
endfunction(lm_helper_find_runtimes)