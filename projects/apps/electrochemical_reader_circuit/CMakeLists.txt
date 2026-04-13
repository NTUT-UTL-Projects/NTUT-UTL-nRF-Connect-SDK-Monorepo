cmake_minimum_required(VERSION 3.20.0)

if(NOT APP_BUILD_HOST_DEMO)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
else()
    add_library(app)
endif()

project(electrochemical_reader_circuit)

include(FetchContent)

if(APP_BUILD_JOHNNY_OVAL_BOARD)
    add_subdirectory(boards)
    add_subdirectory(boards/johnny_oval_board)
endif()


if(APP_BUILD_HOST_DEMO)
    add_subdirectory(demo)
endif()

add_subdirectory(src)
