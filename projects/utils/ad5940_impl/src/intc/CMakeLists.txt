
if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(zephyr)
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
