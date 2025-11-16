cmake_minimum_required(VERSION 3.31)
project(A2_dining_philosophers C)

set(CMAKE_C_STANDARD 11)

add_executable(A2_dining_philosophers
        main.c
        philosopher.h
        philosopher.c
        philosopher.h
        philosopher.c
)