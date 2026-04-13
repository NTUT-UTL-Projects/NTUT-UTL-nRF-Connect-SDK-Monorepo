if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    zephyr_library_include_directories(
        .
        ./debounce
        ./debounce/zephyr
    )
    zephyr_library_sources(
        ./debounce/zephyr/gpio_debounce_impl_zephyr.c
    )
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
