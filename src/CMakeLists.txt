arduino_cmake_project(deer-hunter BOARD_NAME uno)

get_board_id(board_id uno atmega328)

include_directories(
    ${CMAKE_BINARY_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../3rd-party/jeelib
)

add_custom_command(
    OUTPUT moa.raw
    COMMAND "sox"
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/moa.wav -e unsigned-integer -c 1 -r 8000 -b 8 moa.raw
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/moa.wav
)

add_custom_command(
    OUTPUT roy.raw
    COMMAND "sox"
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/roy.wav -e unsigned-integer -c 1 -r 8000 -b 8 roy.raw
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/../data/roy.wav
)

add_custom_command(
    OUTPUT sample.h
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/bin-to-c-source.py"
            bark.raw
            moa
        > sample.h
    DEPENDS
        moa.raw
        roy.raw
        "${CMAKE_CURRENT_SOURCE_DIR}/bin-to-c-source.py"
)

add_arduino_executable(deer-hunter
    ../3rd-party/jeelib/Ports.cpp
    deer-hunter.cc
    pcm.c
    sample.h)

set_target_upload_port(deer-hunter /dev/tty.usbmodem1431)
