cmake_minimum_required(VERSION 3.17)
project(C_Shell C)

set(CMAKE_C_STANDARD 99)

add_executable(C_Shell main.c built_in_functions.h)