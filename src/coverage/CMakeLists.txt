set(CMAKE_MODULE_PATH ${BE_SOURCE_DIR}/coverage/src/)
if(UNIX)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    add_definitions(-fprofile-arcs -ftest-coverage)
    set(COVERAGE_EXCLUDES
      '/usr/include/*'
      '/Applications/*'
    )

    SETUP_TARGET_FOR_COVERAGE(
        NAME coverage         # New target name
        EXECUTABLE tests      # Executable in PROJECT_BINARY_DIR
    )
endif()
