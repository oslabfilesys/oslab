cmake_minimum_required(VERSION 3.7)
project(oslab)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES ./oslab/FILESYS.h oslab/main.cpp oslab/command_parse.cpp oslab/command_parse.h)
add_executable(debug ${SOURCE_FILES})