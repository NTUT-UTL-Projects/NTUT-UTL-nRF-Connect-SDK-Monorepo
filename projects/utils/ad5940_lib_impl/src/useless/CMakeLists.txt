file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")

target_sources(
    ad5940_lib
    PRIVATE
        ${C_FILES}
)
