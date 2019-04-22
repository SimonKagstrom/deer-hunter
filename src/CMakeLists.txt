arduino_cmake_project(deer-hunter BOARD_NAME uno)

add_arduino_executable(deer-hunter deer-hunter.cc)

set_target_upload_port(deer-hunter /dev/ttyACM0)
