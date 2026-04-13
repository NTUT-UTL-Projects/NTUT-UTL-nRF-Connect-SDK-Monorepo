if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    zephyr_library_include_directories(
        .
        ./zephyr
    )
    zephyr_library_sources(
        ./zephyr/spi_impl_zephyr.c
    )
elseif(CONFIG_MY_BUILD_STM32)
elseif(CONFIG_MY_BUILD_FAKE_IMPL)
else()
    message(FATAL_ERROR "Unsupported MCU configuration")
endif()
