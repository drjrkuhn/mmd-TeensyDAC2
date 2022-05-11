cmake_minimum_required(VERSION 3.8.0)
project(TeensyDAC2Firmware VERSION 0.1.0)

MESSAGE( WARNING "Please build ${PROJECT_NAME} with .vsteensy/make" )

add_executable(${PROJECT_NAME} EXCLUDE_FROM_ALL src/main.cpp makefile)
target_include_directories(${PROJECT_NAME} PRIVATE ../lib/ArduinoJson/src ../lib/Ardulingua/src ../lib/SPI)
