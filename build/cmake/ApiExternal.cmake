add_subdirectory(${SAMP_API_EXTERNAL}/boost)

###################################################
# Boost beast
###################################################
SET(BOOST_BEAST_PATH ${SAMP_API_EXTERNAL}/boost-beast)

add_library(boost_beast INTERFACE)
add_library(boost::beast ALIAS boost_beast)

target_include_directories(boost_beast INTERFACE
  $<BUILD_INTERFACE:${BOOST_BEAST_PATH}/include>
)

add_subdirectory(${SAMP_API_EXTERNAL}/fmt)
add_subdirectory(${SAMP_API_EXTERNAL}/spdlog)

option(SPOTIFY_JSON_BUILD_TESTS "Build tests and benchmarks" OFF)
add_subdirectory(${SAMP_API_EXTERNAL}/spotify-json)

##################################################
# SAMPGDK Settings
##################################################

# SAMPSDK path
SET(SAMPSDK_PATH ${SAMP_API_EXTERNAL}/sampsdk)
SET(SAMPGDK_PATH ${SAMP_API_EXTERNAL}/sampgdk)

# Include sampgdk cmake modules
set(CMAKE_MODULE_PATH
	${CMAKE_MODULE_PATH}
	${SAMPGDK_PATH}/cmake
)

# External bin path
set(SAMP_GEN_EXTERNAL ${SAMP_API_EXTERNAL_BIN})

# Set sampgdk settings
set(SAMPGDK_DIR ${SAMPGDK_PATH})
set(SAMPSDK_DIR ${SAMPSDK_PATH})
set(SAMPSDK_INCLUDE_DIR ${SAMPSDK_DIR})
set(SAMPGDK_GEN_DIR ${SAMP_GEN_EXTERNAL}/sampgdk)


# Options
option(SAMPGDK_STATIC "Build static library instead" ON)
option(SAMPGDK_BUILD_PLUGINS "Build example plugins" OFF)
option(SAMPGDK_BUILD_AMALGAMATION "Build amalgamation" OFF)
option(SAMPGDK_BUILD_DOCS "Build Doxygen documentation" ${SAMP_API_BUILD_DOCS})

# Add src
add_subdirectory(${SAMPGDK_PATH})

# Directories
target_include_directories(sampgdk PUBLIC ${SAMPGDK_DIR}/include)
target_include_directories(sampgdk PUBLIC ${SAMPGDK_GEN_DIR}/include/sampgdk)

target_include_directories(sampgdk PUBLIC ${SAMPSDK_INCLUDE_DIR})
if (WIN32)
	target_compile_definitions(sampgdk PRIVATE WIN32)
else()
	target_include_directories(sampgdk PUBLIC ${SAMPSDK_INCLUDE_DIR}/amx)
	target_compile_definitions(sampgdk PRIVATE LINUX)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set_property(TARGET sampgdk APPEND_STRING PROPERTY COMPILE_FLAGS " -m32")
	set_property(TARGET sampgdk APPEND_STRING PROPERTY LINK_FLAGS    " -m32")
endif()

##################################################
# AMXLib / sampsdk
##################################################

# Definitions
add_definitions(-DSAMPSDK_INCLUDE_DIR=${SAMPSDK_INCLUDE_DIR})
add_definitions(-DSAMPSDK_DIR=${SAMPSDK_DIR})

add_library(sampsdk 
	${SAMPSDK_DIR}/amxplugin.cpp
	${SAMPSDK_DIR}/plugin.h
	${SAMPSDK_DIR}/plugincommon.h
)

target_include_directories(sampsdk PUBLIC ${SAMPSDK_INCLUDE_DIR})
if (WIN32)
	target_compile_definitions(sampsdk PRIVATE WIN32)
else()
	target_include_directories(sampsdk PUBLIC ${SAMPSDK_INCLUDE_DIR}/amx)
	target_compile_definitions(sampsdk PRIVATE LINUX)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set_property(TARGET sampsdk APPEND_STRING PROPERTY COMPILE_FLAGS " -m32")
	set_property(TARGET sampsdk APPEND_STRING PROPERTY LINK_FLAGS    " -m32")
endif()

