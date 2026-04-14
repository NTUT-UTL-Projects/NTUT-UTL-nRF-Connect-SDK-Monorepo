file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")

target_sources(
    ad5940_impl
    PUBLIC
        ${SRC_C_FILES}
)
