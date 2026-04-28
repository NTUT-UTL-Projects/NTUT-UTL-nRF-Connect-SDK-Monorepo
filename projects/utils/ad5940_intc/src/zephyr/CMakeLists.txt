file(GLOB C_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

zephyr_library_include_directories(
    .
)
zephyr_library_sources(
    ${C_FILES}
)
target_link_libraries(
    app
    PRIVATE
        ad5940_intc
)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
add_subdirectory(${ROOT_DIR}/utils/gpio/src gpio)
