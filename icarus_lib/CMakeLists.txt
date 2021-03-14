cmake_minimum_required(VERSION 3.12)

project(icarus_lib VERSION "0.0.1" DESCRIPTION "Library used to power the icarus api and mear")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/icarus.cpp)


add_library(icarus_lib SHARED ${SOURCES})

target_include_directories(icarus_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
