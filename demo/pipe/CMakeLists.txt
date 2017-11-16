cmake_minimum_required(VERSION 3.8)
project(pipe)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES pipeOne.c tubeShell.c reader.c writer.c.c)
add_executable(pipe ${SOURCE_FILES})