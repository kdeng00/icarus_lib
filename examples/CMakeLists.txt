cmake_minimum_required(VERSION 3.13)

# NOTE: Possibly might have to set a cmake_policy


# if(NOT DEFINED VCPKG_ROOT_PATH)
    # set(VCPKG_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/vcpkg)
# endif()

# message("The vcpkg root path ${VCPKG_ROOT_PATH}")


# if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    # set(TOOLCHAIN_PATH "${VCPKG_ROOT_PATH}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Toolchain path")

    # set(CMAKE_TOOLCHAIN_FILE
        # "${TOOLCHAIN_PATH}"
        # CACHE STRING "Vcpkg toolchain file")

    # message("Using default toolchain file from VCPKG_ROOT_PATH variable ${CMAKE_TOOLCHAIN_FILE}")
# else()
    # message("Using toolchain file ${CMAKE_TOOLCHAIN_FILE}")
# endif()



# if(NOT DEFINED VCPKG_TRIPLET)
#     message("System name: ${CMAKE_HOST_SYSTEM_NAME}")
# 
#     if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Darwin")
#         set(MACOSX TRUE)
#         set(VCPKG_TRIPLET "x64-osx")
#     elseif (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
#         # Adding Visual C++ support
#         # Has not been implemented
#         set(WINDOWS TRUE)
#         set(VCPKG_TRIPLET "x64-windows")
#     else ()
#         set(VCPKG_TRIPLET "x64-linux")
#     endif()
# 
#     message("Using default ${VCPKG_TRIPLET} triplet")
# endif()

project(examples)


set(ICARUS_LIB_ROOT
#     "${CMAKE_CURRENT_LIST_DIR}/..")
    "${CMAKE_CURRENT_SOURCE_DIR}/..")
message("Root of icarus_lib ${ICARUS_LIB_ROOT}")

set(ICARUS_LIB_INCLUDE "${ICARUS_LIB_ROOT}/include") 
message("icarus_lib include ${ICARUS_LIB_INCLUDE}")

include_directories(${ICARUS_LIB_INCLUDE} ${ICARUS_LIB_ROOT}/examples)

set(ICARUS_LIB_BUILD_OUTPUT ${CMAKE_BINARY_DIR}/subdirs)

# add_subdirectory(${ICARUS_LIB_ROOT} ${ICARUS_LIB_BUILD_OUTPUT})

# TODO: Add examples that cover fetching, updating, creating, and deleting

set(EXAMPLES
#     create_users
#     delete_users
#     fetch_albums
#     fetch_songs
    fetch_users
#     update_users
)

foreach (example ${EXAMPLES})
    message("Compiling ${example}...")

    add_executable(${example} ${example}/${example}.cpp)
    target_link_libraries(${example} PRIVATE icarus_lib)
endforeach()

add_custom_target(examples ALL DEPENDS ${EXAMPLES})
