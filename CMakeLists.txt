cmake_minimum_required(VERSION 2.8)

project(HttpSli)
file(GLOB_RECURSE SOURCES src/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ./src")

add_executable(http_sli ${SOURCES})	