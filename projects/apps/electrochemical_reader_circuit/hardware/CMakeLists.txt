file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB SRC_H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    app
    PRIVATE
        ${SRC_C_FILES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${SRC_H_FILES}
)

if(CONFIG_MY_BUILD_ZEPHYR_BASE)
    add_subdirectory(nrf_boards)
endif()
