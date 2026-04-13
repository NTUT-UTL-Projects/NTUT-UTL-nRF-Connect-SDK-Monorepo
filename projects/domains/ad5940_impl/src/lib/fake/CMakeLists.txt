file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")

target_sources(
    ad5940_impl
    PRIVATE
        ${SRC_C_FILES}
)
