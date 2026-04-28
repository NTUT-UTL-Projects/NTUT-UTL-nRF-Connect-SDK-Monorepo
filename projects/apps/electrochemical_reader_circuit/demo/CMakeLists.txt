add_executable(demo)

target_compile_options(
    demo
    PRIVATE
        -Wall
        -Werror
        -O3
)

# Sources
target_sources(
    demo
    PRIVATE
        main.c
)

target_compile_definitions(
    demo
        PUBLIC
            -D CONFIG_MY_BUILD_FAKE_IMPL
)

# Subdirectories
add_subdirectory(libs)
add_subdirectory(utils)
