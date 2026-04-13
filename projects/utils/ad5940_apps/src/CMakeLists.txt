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

if(NOT TARGET ad5940_apps)
    add_library(ad5940_apps)
endif()

target_sources(
    ad5940_apps
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

# Subdirectories
target_precompile_headers(
    ad5940_apps
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/ad5940_apps.h
)

target_include_directories(
    ad5940_apps
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_subdirectory(commons)
add_subdirectory(consts)
add_subdirectory(str)
add_subdirectory(utils)
