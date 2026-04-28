file(GLOB C_FILES CONFIGURE_DEPENDS "*.c")
set(C_BASENAMES "")
foreach(FILE_PATH ${C_FILES})
    cmake_path(GET FILE_PATH FILENAME BASENAME)
    list(APPEND C_BASENAMES ${BASENAME})
endforeach()

file(GLOB H_FILES CONFIGURE_DEPENDS "*.h")
set(H_BASENAMES "")
foreach(FILE_PATH ${H_FILES})
    cmake_path(GET FILE_PATH FILENAME BASENAME)
    list(APPEND H_BASENAMES ${BASENAME})
endforeach()

target_sources(
    app
    PRIVATE
        ${C_BASENAMES}
    PUBLIC
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS
            ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
            ${H_BASENAMES}
)

add_subdirectory(init)
add_subdirectory(received_data)
