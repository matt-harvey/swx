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

# Custom configuration options

option (
	ENABLE_LOGGING
	"Enable logging (ON/OFF)?"
	ON
)
option (
	ENABLE_ASSERTION_LOGGING
	"Enable logging of assertion failures (ON/OFF)?"
	ON
)
option (
	ENABLE_EXCEPTION_LOGGING
	"Enable logging of thrown exceptions (ON/OFF)?"
	ON
)

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
if (ENABLE_LOGGING)
	add_definitions (-DJEWEL_ENABLE_LOGGING)
endif ()
if (ENABLE_ASSERTION_LOGGING)
	add_definitions (-DJEWEL_ENABLE_ASSERTION_LOGGING)
endif ()
if (ENABLE_EXCEPTION_LOGGING)
	add_definitions (-DJEWEL_ENABLE_EXCEPTION_LOGGING)
endif ()
if (CMAKE_COMPILER_IS_GNUCXX)
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
endif ()

# Find dependencies

find_package (
	Boost 1.53.0
	COMPONENTS
		date_time
		filesystem
		locale
		system
	REQUIRED
)
# find_library (SQLOXX_LIBRARY sqloxx REQUIRED)
find_library (JEWEL_LIBRARY jewel REQUIRED)
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
#if (SQLOXX_LIBRARY-NOTFOUND)
#	message ("Could not find Sqloxx library.")
#endif ()
if (JEWEL_LIBRARY-NOTFOUND)
	message ("Could not find Jewel library.")
endif ()
#if (UNIT_TEST_LIBRARY-NOTFOUND)
#	message ("Could not find UnitTest++ - cannot build tests.")
#endif ()

include_directories(
	include
	${JEWEL_INCLUDES}
	# ${SQLOXX_INCLUDES}
	${Boost_INCLUDE_DIRS}
	# ${UNIT_TEST_INCLUDES}
)

set(
	libraries
	${JEWEL_LIBRARY}
	# ${SQLOXX_LIBRARY}
	${Boost_LIBRARIES}
	${extra_libraries}
)

set(
	common_sources
	src/activity.cpp
	src/interval.cpp
	src/time_log.cpp
)

add_library(swx_common ${common_sources})

# Building the tests
# goes here...

# Building the main executable

add_executable(${executable_stem_name} src/main.cpp)
target_link_libraries(${executable_stem_name} swx_common ${libraries})
