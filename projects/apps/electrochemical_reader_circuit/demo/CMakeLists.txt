add_executable(demo)

target_compile_options(
    demo
    PRIVATE
        -Wall
        -Werror
        -O3
)

# Sources
file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")
file(GLOB H_FILES CONFIGURE_DEPENDS "*.h")

target_sources(
    demo
    PRIVATE
        ${C_FILES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${H_FILES}
)

# Libraries
target_link_libraries(
    demo
    PRIVATE
        app
)

target_compile_definitions(
    app
        PUBLIC -D CONFIG_MY_BUILD_FAKE_IMPL
)
