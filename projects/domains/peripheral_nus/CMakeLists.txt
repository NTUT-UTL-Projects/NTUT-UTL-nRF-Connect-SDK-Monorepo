cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(peripheral_nus)

if (CONFIG_MY_COMMON_DEMO)
    add_subdirectory(demo/common)
endif()

if (CONFIG_MY_RECEIVED_DATA_DEMO)
    add_subdirectory(demo/received_data)
endif()

add_subdirectory(src)
