cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(ad5940_electrochemical_module)

get_filename_component(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR} ABSOLUTE)

# Utils
set(UTILS_DIR ${PROJECT_DIR}/../../utils)

add_definitions(-DCHIPSEL_594X)
set(AD5940_DIR ${UTILS_DIR}/driver/ad5940)
include(${AD5940_DIR}/cmake/ad5940.cmake)
import_ad5940(app ${AD5940_DIR})

add_subdirectory(${UTILS_DIR}/kernel_sleep kernel_sleep)
target_link_libraries(app PRIVATE kernel_sleep)

add_subdirectory(${UTILS_DIR}/gpio gpio)
target_link_libraries(app PRIVATE gpio)

add_subdirectory(${UTILS_DIR}/spi spi)
target_link_libraries(app PRIVATE spi)

# Domains
set(DOMAINS_DIR ${PROJECT_DIR}/../../domains)

set(AD5940_IMPL_DIR ${DOMAINS_DIR}/ad5940_impl)
include(${AD5940_IMPL_DIR}/cmake/ad5940_impl.cmake)
import_ad5940_impl(app ${AD5940_IMPL_DIR})

set(AD5940_CTRL_DIR ${DOMAINS_DIR}/ad5940_controller)
include(${AD5940_CTRL_DIR}/cmake/ad5940_controller.cmake)
import_ad5940_controller(app ${AD5940_CTRL_DIR})

# NORDIC SDK APP START
target_sources(app PRIVATE
  ./src/main.c
  ./src/hardware/ad5940_hardware.c
  ./src/hardware/ad5940_pins_config_impl_zephyr.c
  ./src/lock/zephyr/commands_start_lock_impl_zephyr.c
  ./src/lock/zephyr/commands_stop_lock_impl_zephyr.c
)

target_include_directories(app PRIVATE
  ./src
  ./src/hardware
  ./src/lock
  ./src/lock/zephyr
)
