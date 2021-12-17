
project(examples)


set(ICARUS_LIB_ROOT
    "${CMAKE_CURRENT_SOURCE_DIR}/..")
message("Root of icarus_lib ${ICARUS_LIB_ROOT}")

set(ICARUS_LIB_INCLUDE "${ICARUS_LIB_ROOT}/include") 
message("icarus_lib include ${ICARUS_LIB_INCLUDE}")

include_directories(${ICARUS_LIB_INCLUDE} ${ICARUS_LIB_ROOT}/examples)

set(ICARUS_LIB_BUILD_OUTPUT ${CMAKE_BINARY_DIR}/subdirs)

# TODO: Add examples that cover fetching, updating, creating, and deleting

set(EXAMPLES
    create_users
    delete_users
    fetch_albums
    fetch_songs
    fetch_users
    update_users
)

foreach (example ${EXAMPLES})
    message("Compiling ${example}...")

    add_executable(${example} ${example}/${example}.cpp)
    target_link_libraries(${example} PRIVATE icarus_lib)
endforeach()

add_custom_target(examples ALL DEPENDS ${EXAMPLES})
