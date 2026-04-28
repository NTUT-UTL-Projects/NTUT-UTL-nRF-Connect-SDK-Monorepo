file(GLOB SRC_C_FILES CONFIGURE_DEPENDS "*.c")

target_sources(
    app
    PUBLIC
        ${SRC_C_FILES}
)
