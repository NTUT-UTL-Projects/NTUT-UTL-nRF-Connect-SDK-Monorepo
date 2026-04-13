target_sources(
    app
    PRIVATE
        main.c
)

if(CONFIG_MY_BUILD_JOHNNY_OVAL_BOARD)
    add_subdirectory(johnny_oval_board)
endif()
