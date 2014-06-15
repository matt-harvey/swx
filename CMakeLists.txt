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

cmake_minimum_required(VERSION 2.8.10)


# Project and version

set(application_name "swx")
set(executable_name "swx")
set(file_extension ".swx")
project(${application_name})
set(version_major 0)
set(version_minor 8)
set(version_patch 1)


# Definitions and flags to be passed to the compiler

add_definitions(
	-DSWX_APPLICATION_NAME="${application_name}"
	-DSWX_FILE_EXTENSION="${file_extension}"
	-DSWX_VERSION_MAJOR=${version_major}
	-DSWX_VERSION_MINOR=${version_minor}
	-DSWX_VERSION_PATCH=${version_patch}
	# -DSWX_RELEASE  # uncomment for release version
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


# Dependencies

find_package(Boost 1.53.0 COMPONENTS regex REQUIRED)
include_directories(include ${Boost_INCLUDE_DIRS})
set(libraries ${Boost_LIBRARIES} pthread dl)
set(
	common_sources
	src/atomic_writer.cpp
	src/print_command.cpp
	src/between_command.cpp
	src/command.cpp
	src/command_manager.cpp
	src/config.cpp
	src/config_command.cpp
	src/csv_list_report_writer.cpp
	src/csv_summary_report_writer.cpp
	src/csv_utilities.cpp
	src/edit_command.cpp
	src/file_utilities.cpp
	src/help_command.cpp
	src/help_line.cpp
	src/human_list_report_writer.cpp
	src/human_summary_report_writer.cpp
	src/info.cpp
	src/interval.cpp
	src/list_report_writer.cpp
	src/placeholder.cpp
	src/recording_command.cpp
	src/report_writer.cpp
	src/reporting_command.cpp
	src/resume_command.cpp
	src/round.cpp
	src/since_command.cpp
	src/stint.cpp
	src/stream_flag_guard.cpp
	src/string_utilities.cpp
	src/summary_report_writer.cpp
	src/switch_command.cpp
	src/today_command.cpp
	src/time_point.cpp
	src/time_log.cpp
	src/until_command.cpp
	src/version_command.cpp
)
add_library(swx_common ${common_sources})


# Build the main executable

add_executable(${executable_name} src/main.cpp)
target_link_libraries(${executable_name} swx_common ${libraries})


# Installation

install(
	TARGETS
		${executable_name}
	RUNTIME DESTINATION
		"${CMAKE_INSTALL_PREFIX}/bin"
)
