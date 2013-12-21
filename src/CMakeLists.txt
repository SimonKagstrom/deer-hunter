set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyACM0) # Default Port, when not specified

#====================================================================#
# Advanced firwmare example
#====================================================================#
generate_arduino_firmware(deer-hunter
    SRCS deer-hunter.cc
    BOARD uno)
