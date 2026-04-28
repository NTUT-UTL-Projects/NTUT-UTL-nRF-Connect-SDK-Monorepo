file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")

zephyr_library_include_directories(
    .
)
zephyr_library_sources(
    ${C_FILES}
)

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)
add_subdirectory(${ROOT_DIR}/utils/spi/src spi)
