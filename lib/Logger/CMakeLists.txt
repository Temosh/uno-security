set(logger_SRCS
        src/Logger.cpp
        )
set(logger_SRCS_BOARD ${${PROJECT_NAME}_BOARD})
generate_arduino_library(logger)