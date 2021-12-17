cmake_minimum_required(VERSION 3.13)


if(NOT DEFINED VCPKG_ROOT_PATH)
    set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)
endif()

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
set(CMAKE_CXX_STANDARD 17)


# set(SOURCES
    # ${CMAKE_CURRENT_LIST_DIR}/src/icarus.cpp)


# set(LIBMYSQL_MAIN_H
    # "${VCPKG_ROOT_PATH}/packages/libmysql_${VCPKG_TRIPLET}/include")

# message("MySQL path ${LIBMYSQL_MAIN_H}")

# find_package(libmysql REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)


# if(NOT DEFINED ICARUS_DATA_BUILD_OUTPUT)
    # set(SOCI_CONFIG_H_FILE
        # "${CMAKE_BINARY_DIR}/libraries/icarus_data/3rdparty/soci/include")
# else()
    # set(SOCI_CONFIG_H_FILE
        # "${ICARUS_DATA_BUILD_OUTPUT}/3rdparty/soci/include")
# endif()


# message("soci-config path ${SOCI_CONFIG_H_FILE}")

# Don't remember if I need this. Will eventually find out
# include_directories("${CMAKE_CURRENT_LIST_DIR}/include")


# add_library(icarus_lib SHARED ${SOURCES})
add_library(icarus_lib INTERFACE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

target_include_directories(icarus_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
# target_link_libraries(icarus_lib PUBLIC ${MYSQL_LIBRARIES})
# target_link_libraries(icarus_lib PUBLIC soci_core soci_mysql)
# target_include_directories(icarus_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include ${SOCI_CONFIG_H_FILE})
# target_include_directories(icarus_lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
# target_include_directories(icarus_lib PUBLIC ${LIBMYSQL_MAIN_H})

target_link_libraries(icarus_lib INTERFACE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(icarus_lib INTERFACE CURL::libcurl)
target_link_libraries(icarus_lib INTERFACE cpr)
target_include_directories(icarus_lib INTERFACE ${JWT_CPP_INCLUDE_DIRS})
target_link_libraries(icarus_lib INTERFACE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(icarus_lib INTERFACE unofficial::libmariadb)
target_link_libraries(icarus_lib INTERFACE SOCI::soci_core_static SOCI::soci_empty_static)

