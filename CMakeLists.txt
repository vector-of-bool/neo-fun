cmake_minimum_required(VERSION 3.25)
project(neo-fun VERSION 0.11.1 LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE TESTS "src/*.test.cpp")
list(REMOVE_ITEM SOURCES ${TESTS})

add_library(neo-fun ${SOURCES})
target_include_directories(neo-fun PUBLIC src)
add_library(neo::fun ALIAS neo-fun)
