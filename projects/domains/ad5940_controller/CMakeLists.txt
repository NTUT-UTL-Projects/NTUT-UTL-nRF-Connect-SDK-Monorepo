cmake_minimum_required(VERSION 3.23)

project(ad5940_controller)

include(FetchContent)

# Parameters
option(AD5940_CONTROLLER_BUILD_DEMO "AD5940 Controller Build Demo" OFF)
option(AD5940_CONTROLLER_BUILD_TESTS "AD5940 Controller Build Tests" OFF)
option(BUILD_FAKE "Build with fake MCU" OFF)

# Subdirectories
add_subdirectory(src)

# Test
if(AD5940_CONTROLLER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Demo
if(AD5940_CONTROLLER_BUILD_DEMO)
    add_subdirectory(demo)
endif()
