set(CMAKE_TOOLCHAIN_FILE cmake/Arduino-Toolchain.cmake) # Arduino Toolchain

cmake_minimum_required(VERSION 2.8)
#====================================================================#
#  Setup Project                                                     #
#====================================================================#
project(DeerHunter C CXX)

add_subdirectory(src)
