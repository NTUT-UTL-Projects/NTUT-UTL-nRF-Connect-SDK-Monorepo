cmake_minimum_required(VERSION 3.20.0)

if(NOT MY_BUILD_HOST_DEMO)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
else()
    add_library(app)
endif()

project(electrochemical_reader_circuit)

message("AAAAA ${ZEPHYR_BASE}")
message("BBBBB ${CONFIG_ZEPHYR_BASE}")

include(FetchContent)

if(CONFIG_MY_BUILD_JOHNNY_OVAL_BOARD)
    add_subdirectory(boards)
endif()

if(MY_BUILD_HOST_DEMO)
    add_subdirectory(demo)
endif()

add_subdirectory(src)
