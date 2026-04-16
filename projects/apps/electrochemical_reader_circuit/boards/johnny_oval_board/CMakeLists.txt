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

# Utils
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

# peripheral_nus
add_subdirectory(
    ${ROOT_DIR}/domains/peripheral_nus/src
    peripheral_nus
)

# watchdog
add_subdirectory(
    ${ROOT_DIR}/domains/watchdog/src
    watchdog
)

# add_subdirectory(
#     ${ROOT_DIR}/utils/gpio/src
#     gpio
# )
# target_link_libraries(app PRIVATE gpio)

# add_subdirectory(
#     ${ROOT_DIR}/utils/spi/src
#     spi
# )
# target_link_libraries(app PRIVATE spi)
