cmake_minimum_required(VERSION 3.8)
project(tp2)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES serveur.c)
add_executable(tp2 ${SOURCE_FILES})