cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
project(task3w1)
message(INFO:\ ${task3w1_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${task3w1_SOURCE_DIR}/bin)

include_directories(${task3w1_SOURCE_DIR}/include)
add_executable(task3w1 main.cpp src/ans_handler.cpp src/my_math.cpp) 
