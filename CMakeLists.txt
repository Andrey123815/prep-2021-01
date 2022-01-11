cmake_minimum_required(VERSION 3.17)
project(prep-2021.01)

set(CMAKE_C_STANDARD 99)

add_executable(main
        project/src/main.c
        project/src/list.c
        project/src/parser.c
        project/include/parser.h
        project/include/list.h)