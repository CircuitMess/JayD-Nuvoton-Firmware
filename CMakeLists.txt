cmake_minimum_required(VERSION 3.15)
project(JayD-Nuvoton-Firmware)
set(CMAKE_C_STANDARD 99)

# set(CMAKE_C_COMPILER sdcc)
add_definitions(-DN76E616)
add_definitions(-D__SDCC__)

include_directories(${CMAKE_SOURCE_DIR}/inc)


AUX_SOURCE_DIRECTORY(src SOURCES)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/hex)

add_executable(main.hex ${SOURCES})