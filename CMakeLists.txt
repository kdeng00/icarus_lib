cmake_minimum_required(VERSION 3.18)


if(NOT DEFINED VCPKG_ROOT_PATH)
    set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)

    set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT_PATH}/scripts/buildsystem/vcpkg.cmake" CACHE 
        STRING "Vcpkg toolchain file")
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
set(CMAKE_CXX_STANDARD 20)


set(ICARUS_LIB_ROOT 
    "${CMAKE_CURRENT_LIST_DIR}")
message("icarus_lib root ${ICARUS_LIB_ROOT}")

set(SOURCES
    ${ICARUS_LIB_ROOT}/src/icarus_lib/utility/image_file.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/album_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/artist_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/base_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/cover_art_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/genre_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/song_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/token_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/repositories/database/cloud/user_repository.cpp
    ${ICARUS_LIB_ROOT}/src/icarus_lib/utility/password_encryption.cpp)


# include_directories(${CMAKE_CURRENT_LIST_DIR}/include/3rdparty/soci/include)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(VCPKG_INCLUDE_DIR "${VCPKG_ROOT_PATH}/installed/${VCPKG_TRIPLET}/include" CACHE STRING "vcpkg library includes")
message("including ${VCPKG_INCLUDE_DIR}")
include_directories(${VCPKG_INCLUDE_DIR})
# include_directories(${CMAKE_BINARY_DIR}/include/3rdparty/soci/include)
# include_directories(${CMAKE_BINARY_DIR}/icarus_lib/include/3rdparty/soci/include)


# set(SOCI_TESTS OFF CACHE BOOL "enable soci tests" FORCE)
# set(SOCI_EMPTY OFF CACHE BOOL "enable soci empty" FORCE)
# set(WITH_MYSQL ON CACHE BOOL "enable mysql" FORCE)
# set(WITH_ODBC OFF CACHE BOOL "enable ODBC" FORCE)
# set(WITH_SQLITE3 OFF CACHE BOOL "enable sqlite3" FORCE)
# set(WITH_BOOST OFF CACHE BOOL "enable boost" FORCE)
# set(WITH_DB2 OFF CACHE BOOL "enable DB2" FORCE)
# set(WITH_FIREBIRD OFF CACHE BOOL "enable firebird" FORCE)
# set(WITH_ORACLE OFF CACHE BOOL "enable oracle" FORCE)

set(CMAKE_PREFIX_PATH
    # "${VCPKG_ROOT_PATH}/packages/libmysql_x64-linux/share/libmysql"
    "${VCPKG_ROOT_PATH}/packages/libmariadb_x64-linux/share/unofficial-libmariadb")


find_package(OpenSSL REQUIRED)
find_package(CURL CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_path(JWT_CPP_INCLUDE_DIRS "jwt-cpp/base.h")
find_package(nlohmann_json CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
# find_package(taglib CONFIG REQUIRED)
# find_package(libmysql REQUIRED)
find_package(unofficial-libmariadb CONFIG REQUIRED)



# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/include/3rdparty/soci)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
# Add tests
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)


add_library(icarus_lib SHARED ${SOURCES})

# target_include_directories(icarus_lib PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

target_include_directories(icarus_lib PRIVATE ${JWT_CPP_INCLUDE_DIRS})
target_link_libraries(icarus_lib PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(icarus_lib PRIVATE CURL::libcurl)
target_link_libraries(icarus_lib PRIVATE cpr)
target_link_libraries(icarus_lib PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
target_link_libraries(icarus_lib PRIVATE cryptopp-static)
target_link_libraries(icarus_lib PRIVATE unofficial::libmariadb tag)
# target_link_libraries(icarus_lib PRIVATE tag taglib)


if(DEFINED CMAKE_EXPORT_COMPILE_COMMANDS)
	if("${CMAKE_EXPORT_COMPILE_COMMANDS}" STREQUAL "ON")
        message("Adding custom command")

        add_custom_command(TARGET icarus PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_BINARY_DIR}/compile_commands.json" "${CMAKE_SOURCE_DIR}/compile_commands.json"
	        COMMENT "Copied the compile_commands.json file to project root"
	    )
	endif()
endif()

