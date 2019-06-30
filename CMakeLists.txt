cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/cmake/ArduinoToolchain.cmake)
set(CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME uno-security)
set(${PROJECT_NAME}_BOARD uno)
set(${PROJECT_NAME}_SKETCH src/UnoSecurity.ino)
set(${PROJECT_NAME}_PORT COM3)
# set(${PROJECT_NAME}_PROGRAMMER avrispmkii)
# set(${PROJECT_NAME}_AFLAGS -v)
# set(ARDUINO_CPU)
# set(uno.upload.speed 9600)

project(${PROJECT_NAME})

# Libraries
add_subdirectory(lib)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Includes
include_directories(include)

# Search for sources
# file(GLOB_RECURSE SOURCES src/*.cpp)

# set(${PROJECT_NAME}_SRCS ${SOURCES})
# set(${PROJECT_NAME}_HDRS ${HEADERS})
# set(${PROJECT_NAME}_LIBS ${LIBRARIES})
set(${PROJECT_NAME}_ARDLIBS
        keypad
        liquid-crystal-i2c
        logger
        )

generate_arduino_firmware(${PROJECT_NAME})
