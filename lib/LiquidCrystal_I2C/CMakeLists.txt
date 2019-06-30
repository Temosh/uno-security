set(liquid-crystal-i2c_SRCS LiquidCrystal_I2C.cpp)
set(liquid-crystal-i2c_SRCS_BOARD ${${PROJECT_NAME}_BOARD})
generate_arduino_library(liquid-crystal-i2c)