set(ARDUINO_DEFAULT_BOARD uno) # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT /dev/ttyUSB0) # Default Port, when not specified

#====================================================================#
# Advanced firwmare example
#====================================================================#
generate_arduino_firmware(deer-hunter
    SRCS deer-hunter.c
    BOARD uno)
