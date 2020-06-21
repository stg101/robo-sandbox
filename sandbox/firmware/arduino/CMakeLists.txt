project(arduino_firmware VERSION 1.0.0)
include(${CMAKE_SOURCE_DIR}/macros.cmake)

add_custom_target(arduino_firmware)

add_custom_command(
  TARGET arduino_firmware
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/
          ${CMAKE_CURRENT_BINARY_DIR})

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${SUBDIRS})
  add_custom_command(
    TARGET arduino_firmware
    POST_BUILD
    COMMAND arduino-cli compile --fqbn arduino:avr:uno ${subdir}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endforeach()
