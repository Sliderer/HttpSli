cmake_minimum_required(VERSION 3.00)

project(httpsli)

set(CMAKE_C_CSTANDARD 17)

include_directories(src)

file(GLOB_RECURSE FILES
    "src/*.cpp"
    "main.cpp"
    )

add_executable(executable ${FILES})
