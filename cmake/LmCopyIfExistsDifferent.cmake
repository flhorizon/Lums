
# Wrapper for the cmake -E copy_if_different.
# Checks if the copy-source file exists.
# Module executed as a custom command.

if(EXISTS ${SOURCE_FILE})
  execute_process(COMMAND ${CMAKE_COMMAND}
    -E copy_if_different ${SOURCE_FILE} ${DESTINATION_FILE}
)
endif()