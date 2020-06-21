# Project name
project(avr_firmware)

include(${CMAKE_SOURCE_DIR}/macros.cmake)

# AVR Chip Configuration 8Mhz, this should match the crystal on your board, I
# use 8Mhz and 3.3V for the lowest power consumption
set(F_CPU 8000000UL)
# CPU, you can find the list here:
# https://gcc.gnu.org/onlinedocs/gcc/AVR-Options.html
set(MCU atmega328p)
# Default Baudrate for UART, read avr include/util/setbaud.h for usage
set(BAUD 9600)
# The programmer to use, read avrdude manual for list
set(PROG_TYPE avrispmkII)

# AVR Fuses, must be in concordance with your hardware and F_CPU
# http://eleccelerator.com/fusecalc/fusecalc.php?chip=atmega328p
set(E_FUSE 0xfd)
set(H_FUSE 0xda)
set(L_FUSE 0xfd)
set(LOCK_BIT 0xff)

# Use AVR GCC toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER avr-g++)
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_ASM_COMPILER avr-gcc)

# Include simavr directories
include_directories(${CMAKE_SOURCE_DIR}/extern/simavr/simavr/sim/avr)

# Pass defines to compiler
add_definitions(-DF_CPU=${F_CPU} -DBAUD=${BAUD})
# mmcu MUST be passed to bot the compiler and linker, this handle the linker
set(CMAKE_EXE_LINKER_FLAGS -mmcu=${MCU})

add_compile_options(
  -mmcu=${MCU} # MCU
  -std=gnu99 # C99 standard
  -Os # optimize
  -Wall # enable warnings
  -Wno-main
  -Wundef
  -pedantic
  -Werror
  -Wfatal-errors
  -Wl,--relax,--gc-sections
  -g
  -gdwarf-2
  -funsigned-char # a few optimizations
  -funsigned-bitfields
  -fpack-struct
  -fshort-enums
  -ffunction-sections
  -fdata-sections
  -fno-inline-small-functions
  -fno-split-wide-types
  -fno-tree-scev-cprop)

subdirlist(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(avr_firmware)

foreach(subdir ${SUBDIRS})
  # Product filename
  set(PRODUCT_NAME ${subdir})
  file(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/*.c")
  add_executable(${PRODUCT_NAME} ${SRC_FILES})
  add_dependencies(avr_firmware ${PRODUCT_NAME})
  set_target_properties(${PRODUCT_NAME} PROPERTIES OUTPUT_NAME
                                                   ${PRODUCT_NAME}.elf)

  add_custom_command(
    TARGET ${PRODUCT_NAME}
    POST_BUILD
    COMMAND avr-strip ${PRODUCT_NAME}.elf -o ${PRODUCT_NAME}-striped.elf
    COMMAND avr-objcopy -R .eeprom -O ihex ${PRODUCT_NAME}-striped.elf
            ${PRODUCT_NAME}.hex
    COMMAND ${CMAKE_COMMAND} -E make_directory
            ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT_NAME}
    COMMAND mv -t ${CMAKE_CURRENT_BINARY_DIR}/${PRODUCT_NAME}
            ${PRODUCT_NAME}.elf ${PRODUCT_NAME}-striped.elf ${PRODUCT_NAME}.hex
    BYPRODUCTS (${PRODUCT_NAME}-striped.elf ${PRODUCT_NAME}.hex)
    COMMENT "[comment] producing hex")

endforeach()
