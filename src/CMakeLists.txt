cmake_minimum_required(VERSION 3.8.0)

project(BE C CXX)

# Set version number
set(MAJOR 1)
set(MINOR 1)
set(PATCH 0)

# Options
option(BE_BUILD_TEST "whether or not to build the tests" OFF)
option(BE_BUILD_TEST_COVERAGE "whether or not to build the tests coverage" OFF)

# Set the output paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BE_BINARY_DIR}/bin")

# Check C++11 support
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if (COMPILER_SUPPORTS_CXX11)
  if (BE_BUILD_TEST_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fprofile-arcs -ftest-coverage")
  else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  endif ()
else ()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif ()

# Subdirectories to process
add_subdirectory(brassweibull)
# add_subdirectory(cephes)
add_subdirectory(core)
if (BE_BUILD_TEST_COVERAGE)
  add_subdirectory(coverage)
endif ()
add_subdirectory(dcdf)
add_subdirectory(global)
add_subdirectory(homogeneous)
add_subdirectory(nonhomogeneous)
add_subdirectory(parametric)
add_subdirectory(ran)
# add_subdirectory(redcas)
# add_subdirectory(redcassrv)
add_subdirectory(slatec)
if (BE_BUILD_TEST)
    enable_testing()
    add_subdirectory(tests)
endif ()
# add_subdirectory(testsuite)
add_subdirectory(twostage)
