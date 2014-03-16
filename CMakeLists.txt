# Copyright 2014 Matthew Harvey

# Preliminaries

cmake_minimum_required (VERSION 2.8)

# Set project and version

set (application_name "swx")
set (executable_stem_name "swx")
set (file_extension ".swx")
project (${application_name})
set (version_major 1)
set (version_minor 0)
set (version_patch 0)

# Definitions to be passed to the compiler

# NOTE Adding definitions via "add_definitions" can cause problems
# in recent versions of CMake, where these get passed to RC_FLAGS, if
# the definitions are invalid for the RC compiler.

add_definitions (
	-DSWX_APPLICATION_NAME="${application_name}"
	-DSWX_FILE_EXTENSION="${file_extension}"
	-DSWX_VERSION_MAJOR=${version_major}
	-DSWX_VERSION_MINOR=${version_minor}
	-DSWX_VERSION_PATCH=${version_patch}
)
if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif ()

# Find dependencies

# find_library (UNIT_TEST_LIBRARY UnitTest++ REQUIRED)
if (WIN32)
	set (
		extra_libraries
		winmm
		shell32
		comctl32
		rpcrt4
		wsock32
		odbc32
		opengl32
	)
elseif (UNIX)
	set (
		extra_libraries
		pthread
		dl
	)
endif ()
#if (UNIT_TEST_LIBRARY-NOTFOUND)
#	message ("Could not find UnitTest++ - cannot build tests.")
#endif ()

include_directories(
	include
)

set(
	libraries
	${extra_libraries}
)

set(
	common_sources
	src/activity.cpp
	src/command_processor.cpp
	src/interval.cpp
	src/time_conversion.cpp
	src/time_log.cpp
)

add_library(swx_common ${common_sources})

# Building the tests
# goes here...

# Building the main executable

add_executable(${executable_stem_name} src/main.cpp)
target_link_libraries(${executable_stem_name} swx_common ${libraries})
