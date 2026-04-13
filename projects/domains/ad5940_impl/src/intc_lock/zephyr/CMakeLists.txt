file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

zephyr_library_include_directories(
    .
)
zephyr_library_sources(
    ${SRC_C_FILES}
)
