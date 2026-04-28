cmake_minimum_required(VERSION 3.23)

project(ad5940_lib)

include(FetchContent)

# Parameters
option(MY_BUILD_DEMO "AD5940 Library Build Demo" OFF)
option(MY_BUILD_TESTS "AD5940 Library Build Tests" OFF)
option(CONFIG_MY_BUILD_FAKE_IMPL "Build with fake MCU" OFF)

# Subdirectories
add_subdirectory(src)

# Test
if(MY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Demo
if(MY_BUILD_DEMO)
    add_subdirectory(demo)
endif()
