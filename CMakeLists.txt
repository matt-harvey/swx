cmake_minimum_required(
	VERSION 2.6
)

project(
	swx
)

include_directories(
)

link_directories(
	/usr/local/lib
)

add_executable(
	swx
	src/main.cpp
)

target_link_libraries(
	swx
	jewel
	sqloxx
	dl
	m
	z
)

