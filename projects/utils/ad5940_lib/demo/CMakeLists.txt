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
        ad5940_lib
)

target_compile_definitions(
    ad5940_lib
    PUBLIC
        CHIPSEL_594X
)
