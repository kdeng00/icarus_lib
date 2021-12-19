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




include_directories(${CMAKE_CURRENT_LIST_DIR}/include/3rdparty/soci/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(VCPKG_INCLUDE_DIR "${VCPKG_ROOT_PATH}/installed/${VCPKG_TRIPLET}/include")
message("including ${VCPKG_INCLUDE_DIR}")
include_directories(${VCPKG_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR}/include/3rdparty/soci/include)


find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)
# find_package(SOCI CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)


set(SOCI_TESTS OFF CACHE BOOL "enable soci tests" FORCE)
set(SOCI_EMPTY OFF CACHE BOOL "enable soci empty" FORCE)
set(WITH_MYSQL ON CACHE BOOL "enable mysql" FORCE)
set(WITH_ODBC OFF CACHE BOOL "enable ODBC" FORCE)
set(WITH_SQLITE3 OFF CACHE BOOL "enable sqlite3" FORCE)
set(WITH_BOOST OFF CACHE BOOL "enable boost" FORCE)
set(WITH_DB2 OFF CACHE BOOL "enable DB2" FORCE)
set(WITH_FIREBIRD OFF CACHE BOOL "enable firebird" FORCE)
set(WITH_ORACLE OFF CACHE BOOL "enable oracle" FORCE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/include/3rdparty/soci)

set(SOURCES
    src/icarus.cpp)


add_library(icarus_lib SHARED ${SOURCES})

# Add tests
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)

# target_include_directories(icarus_lib INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)

target_link_libraries(icarus_lib PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(icarus_lib PRIVATE CURL::libcurl)
target_link_libraries(icarus_lib PRIVATE cpr)
target_include_directories(icarus_lib PRIVATE ${JWT_CPP_INCLUDE_DIRS})
target_link_libraries(icarus_lib PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(icarus_lib PRIVATE unofficial::libmariadb)
target_link_libraries(icarus_lib PRIVATE soci_core soci_mysql)
target_link_libraries(icarus_lib PRIVATE cryptopp-static)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)

if(DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
	if("${CMAKE_EXPORT_COMPILE_COMMANDS}" STREQUAL "ON")
        message("Adding custom command")

        add_custom_command(TARGET icarus PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/compile_commands.json" "${CMAKE_SOURCE_DIR}/compile_commands.json"
	        COMMENT "Copied the compile_commands.json file to project root"
	    )
	endif()
endif()
