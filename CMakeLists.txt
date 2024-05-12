cmake_minimum_required(VERSION 3.00)

project(httpsli)

set(CMAKE_C_CSTANDARD 17)

# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I ./src")

include_directories(src)

file(GLOB_RECURSE FILES
    "src/*.cpp"
    "main.cpp"
    )

add_executable(executable ${FILES})
