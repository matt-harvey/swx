###
# Copyright 2014 Matthew Harvey
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###


# Preliminaries

cmake_minimum_required (VERSION 2.8.10)


# Set project and version

set (application_name "swx")
set (executable_stem_name "swx")
set (file_extension ".swx")
project (${application_name})
set (version_major 0)
set (version_minor 1)
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

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Find dependencies

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

include_directories(
	include
)

set(
	libraries
	${extra_libraries}
)

set(
	common_sources
	src/today_command.cpp
	src/print_command.cpp
	src/between_command.cpp
	src/command.cpp
	src/command_manager.cpp
	src/config.cpp
	src/help_command.cpp
	src/help_line.cpp
	src/info.cpp
	src/interval.cpp
	src/round.cpp
	src/since_command.cpp
	src/stint.cpp
	src/stream_flag_guard.cpp
	src/string_utilities.cpp
	src/switch_command.cpp
	src/time_point.cpp
	src/time_log.cpp
	src/until_command.cpp
	src/version_command.cpp
)

add_library(swx_common ${common_sources})


# Build the main executable

add_executable(${executable_stem_name} src/main.cpp)
target_link_libraries(${executable_stem_name} swx_common ${libraries})


# Installation

install(
	TARGETS
		${executable_stem_name}
	RUNTIME DESTINATION
		"${CMAKE_INSTALL_PREFIX}/bin"
)
