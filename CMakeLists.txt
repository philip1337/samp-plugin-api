# CMake required version
cmake_minimum_required(VERSION 3.1)

# "Only interpret if() arguments as variables or keywords when unquoted."
cmake_policy(SET CMP0054 NEW)

# Set cmake build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# Options
option(SAMP_API_USE_FOREIGN_EXTERNAL "Use foreign externals" OFF)
option(SAMP_API_INCLUDE_PLUGIN "Include plugin to project" ON)
option(SAMP_API_BUILD_DOCS "Build Doxygen documentation" ON)
option(SAMP_API_USE_BUILD_DIR "Change build directory" ON)

# Set project title
project(samp-plugin-api)

# Base path constants
set(SAMP_API_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SAMP_API_BIN_ROOT ${CMAKE_CURRENT_BINARY_DIR})
set(SAMP_API_BIN_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)

# Set initial external directories
if ("${SAMP_API_EXTERNAL}" STREQUAL "")
	set(SAMP_API_EXTERNAL ${SAMP_API_ROOT}/external)
endif()

if ("${SAMP_API_EXTERNAL_BIN}" STREQUAL "")
	set(SAMP_API_EXTERNAL_BIN ${SAMP_API_BIN_ROOT}/external)
endif()

# Module
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${SAMP_API_ROOT}/build/cmake
)

# Include externals
if (NOT SAMP_API_USE_FOREIGN_EXTERNAL)
	include(ApiExternal)
endif()

# Include other
include(ApiDefines)
include(ApiFunctions)
include(ApiCore)

# Add src
add_subdirectory(src)
