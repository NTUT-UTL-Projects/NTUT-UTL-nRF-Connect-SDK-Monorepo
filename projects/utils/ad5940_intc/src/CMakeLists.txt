if(NOT TARGET ad5940_intc)
    add_library(ad5940_intc INTERFACE)
endif()

# Sources
target_precompile_headers(
    ad5940_intc
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_intc.h
)
target_include_directories(
    ad5940_intc
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

# Subdirectories
if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(zephyr)
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
    add_subdirectory(fake)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
