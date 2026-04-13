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

# Libraries
target_link_libraries(
    demo
    PRIVATE
        app
)

# Subdirectory
add_subdirectory(config)
