arduino_cmake_project(deer-hunter BOARD_NAME uno)

get_board_id(board_id uno atmega328)

include_directories(
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_custom_command(
    OUTPUT bark.raw
    COMMAND "sox"
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/bark.wav -e unsigned-integer -c 1 -r 8000 -b 8 bark.raw
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/bark.wav
)

add_custom_command(
    OUTPUT sample.h
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bin-to-c-source.py"
            bark.raw
            sample
        > sample.h
    DEPENDS
        bark.raw
        "${CMAKE_CURRENT_SOURCE_DIR}/bin-to-c-source.py"
)

add_arduino_executable(deer-hunter
    deer-hunter.cc
    pcm.c
    sample.h)

set_target_upload_port(deer-hunter /dev/tty.usbmodem1431)
