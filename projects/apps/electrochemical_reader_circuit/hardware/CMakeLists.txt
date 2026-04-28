target_include_directories(
    app
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(nrf_boards)
endif()
