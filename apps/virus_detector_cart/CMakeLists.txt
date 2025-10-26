cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(virus_detector_cart)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

# Utils
set(UTILS_DIR ${PROJECT_DIR}/../../utils)

add_subdirectory(${UTILS_DIR}/driver/stepper_motor stepper_motor)
target_link_libraries(app PRIVATE stepper_motor)

target_sources(app PRIVATE 
        src/main.c
        src/valve.c
)

target_include_directories(app PRIVATE
        ./src
)
