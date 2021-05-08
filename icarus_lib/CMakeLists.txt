cmake_minimum_required(VERSION 3.12)


set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)

message("The vcpkg root path ${VCPKG_ROOT_PATH}")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(TOOLCHAIN_PATH ${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake)

    set(CMAKE_TOOLCHAIN_FILE
        "${TOOLCHAIN_PATH}"
        CACHE STRING "Vcpkg toolchain file")

    message("Using default toolchain file")
endif()



if(NOT DEFINED VCPKG_TRIPLET)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(MACOSX TRUE)
        set(VCPKG_TRIPLET "x64-osx")
    else ()
        set(VCPKG_TRIPLET "x64-linux")
    endif()

    message("Using default ${VCPKG_TRIPLET} triplet")
endif()

project(icarus_lib VERSION "0.0.1" DESCRIPTION "Library used to power the icarus api")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/src/icarus.cpp)


add_library(icarus_lib SHARED ${SOURCES})

target_include_directories(icarus_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
