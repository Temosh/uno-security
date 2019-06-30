set(keypad_SRCS
        src/Key.cpp
        src/Keypad.cpp
        )
set(keypad_SRCS_BOARD ${${PROJECT_NAME}_BOARD})
generate_arduino_library(keypad)