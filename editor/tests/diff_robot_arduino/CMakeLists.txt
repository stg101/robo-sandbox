cmake_minimum_required(VERSION 3.12) 
project(ArduinoFirmware VERSION 1.0.0)

add_custom_target(copy)
add_custom_target(compile_arduino_firmware
  ALL
  DEPENDS
    copy
    ${CMAKE_CURRENT_BINARY_DIR}/__header.h
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.elf
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.hex
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.with_bootloader.hex
)

add_custom_command(
  # TARGET   compile_arduino_firmware
  OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/__header.h
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.elf
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.hex
    ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware/arduino_firmware.arduino.avr.uno.with_bootloader.hex
  COMMAND  arduino-cli compile --fqbn arduino:avr:uno arduino_firmware
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})


add_custom_command(
        TARGET copy
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/arduino_firmware/
                ${CMAKE_CURRENT_BINARY_DIR}/arduino_firmware)

