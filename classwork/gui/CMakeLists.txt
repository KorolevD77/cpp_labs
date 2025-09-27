cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(FETCHCONTENT_QUIET FALSE)
set(FONTS_DIR_PATH ${CMAKE_CURRENT_SOURCE_DIR}/fonts)
file(COPY ${FONTS_DIR_PATH} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp)
target_link_libraries(main PRIVATE SFML::Graphics)
