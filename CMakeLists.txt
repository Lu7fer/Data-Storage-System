cmake_minimum_required(VERSION 3.16)
project(Data_Storage_System C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Data_Storage_System
        display.h
        main.c
        utility.h
        varchar.h)
