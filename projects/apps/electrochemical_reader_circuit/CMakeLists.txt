cmake_minimum_required(VERSION 3.20.0)

if(NOT CONFIG_MY_BUILD_HOST_DEMO)
    find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
endif()

project(electrochemical_reader_circuit)

include(FetchContent)

add_subdirectory(src)

if(CONFIG_MY_BUILD_HOST_DEMO)
    add_subdirectory(demo)
else()
    add_subdirectory(hardware)
endif()
