# Sources
target_sources(
    app
    PRIVATE
        main.c
)

# Boards
if(CONFIG_MY_BUILD_JOHNNY_OVAL_BOARD)
    add_subdirectory(johnny_oval_board)
endif()

# Utils
set(
    ROOT_DIR
    ${CMAKE_CURRENT_SOURCE_DIR}/../../../..
)

# peripheral_nus
add_subdirectory(
    ${ROOT_DIR}/domains/peripheral_nus/src
    ${CMAKE_CURRENT_BINARY_DIR}/peripheral_nus
)

# watchdog
add_subdirectory(
    ${ROOT_DIR}/domains/watchdog/src
    ${CMAKE_CURRENT_BINARY_DIR}/watchdog
)

# add_subdirectory(
#     ${ROOT_DIR}/utils/gpio/src
#     ${CMAKE_CURRENT_BINARY_DIR}/gpio
# )
# target_link_libraries(app PRIVATE gpio)

# add_subdirectory(
#     ${ROOT_DIR}/utils/spi/src
#     ${CMAKE_CURRENT_BINARY_DIR}/spi
# )
# target_link_libraries(app PRIVATE spi)
