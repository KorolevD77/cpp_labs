cmake_minimum_required(VERSION 3.20)
#set(CMAKE_C_COMPILER "gcc")

project(main LANGUAGES C)

#set(CMAKE_CXX_STANDARD 17)
add_executable(main main.c tetris.c)

target_include_directories(main PRIVATE .)