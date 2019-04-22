arduino_cmake_project(deer-hunter BOARD_NAME uno)

get_board_id(board_id uno atmega328)

add_arduino_executable(deer-hunter
    deer-hunter.cc
    pcm.c)

set_target_upload_port(deer-hunter /dev/tty.usbmodem1431)
