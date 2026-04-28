add_executable(demo)

target_compile_options(
    demo
    PRIVATE
        -Wall
        -Werror
        -O3
)

# Libraries
target_link_libraries(
    demo
    PRIVATE
        ad5940_intc_impl
)

# Sources
target_sources(
    demo
    PRIVATE
        main.c
)
