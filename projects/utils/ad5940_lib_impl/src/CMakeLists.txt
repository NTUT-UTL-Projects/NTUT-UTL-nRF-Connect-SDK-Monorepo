# Library
set(
    ROOT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../../..
)
if(NOT TARGET ad5940_lib)
    add_subdirectory(
        ${ROOT_DIR}/utils/ad5940_lib/src
        ${CMAKE_CURRENT_BINARY_DIR}/aad5940_lib
    )
endif()

# Subdirectories
add_subdirectory(useless)

if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(zephyr)
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
    add_subdirectory(fake)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
