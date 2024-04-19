cmake_minimum_required(VERSION 3.25)

project(cppdiff VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -Wextra -Wpedantic")

add_executable(
    cppdiff
    src/main.cpp
)
