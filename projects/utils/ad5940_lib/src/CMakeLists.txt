if(NOT TARGET ad5940_lib)
    add_library(ad5940_lib)
endif()

# Library
FetchContent_GetProperties(ad5940lib)
if(NOT ad5940lib_POPULATED)
    FetchContent_Declare(
        ad5940lib
        GIT_REPOSITORY  https://github.com/analogdevicesinc/ad5940lib.git
        GIT_SHALLOW     TRUE
    )
    FetchContent_Populate(ad5940lib)
endif()

target_sources(
    ad5940_lib
    PRIVATE
        ${ad5940lib_SOURCE_DIR}/ad5940.c
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${ad5940lib_SOURCE_DIR}
        FILES
            ${ad5940lib_SOURCE_DIR}/ad5940.h
)
target_precompile_headers(
    ad5940_lib
    PUBLIC
        ${ad5940lib_SOURCE_DIR}/ad5940.h
)

# Math library
find_library(m m)
if(m)
    target_link_libraries(
        ad5940_lib
        PUBLIC
            m
    )
endif()
