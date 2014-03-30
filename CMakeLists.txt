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

	# Controls format in which dates are stored in data file.
	# If you change this, remember also to change SWX_FORMATTED_BUF_LEN
	# accordingly.
	-DSWX_FORMAT_STRING="%Y-%m-%dT%H:%M:%S"

	# Must be at least 1 greater than the length of the longest
	# string that could result from formatting a date using
	# SWX_FORMAT_STRING
	-DSWX_FORMATTED_BUF_LEN=20
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
	src/print_command_processor.cpp
	src/command_processor.cpp
	src/command_router.cpp
	src/day.cpp
	src/help_command_processor.cpp
	src/info.cpp
	src/interval.cpp
	src/round.cpp
	src/string_utilities.cpp
	src/switch_command_processor.cpp
	src/time_conversion.cpp
	src/time_log.cpp
	src/version_command_processor.cpp
)

add_library(swx_common ${common_sources})

# Building the tests
# goes here...

# Building the main executable

add_executable(${executable_stem_name} src/main.cpp)
target_link_libraries(${executable_stem_name} swx_common ${libraries})
