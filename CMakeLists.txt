cmake_minimum_required(VERSION 3.20)

set( PROCESSOR "attiny13a")
set( MCU "t13")

set(PROJECT_PATH "src/app/${PROJECT}")

set(CDEFS     "-DF_CPU=${F_CPU} -DBAUD=${BAUD} -D${PROCESSOR}")
set(CSTANDARD "-std=gnu99")
set(CWARN     "-Wall -Wextra -Wstrict-prototypes -Wshadow -Wdouble-promotion -Wundef -fno-common -Wno-unused-but-set-variable -Wno-unused-parameter -Wno-unused-variable")


set(PREFIX avr-)
# Compiler flags
set(CDEBUG    "-gstabs -g -ggdb")
set(CTUNING   "-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -ffunction-sections -fdata-sections")
set(COPT      "-Os -lm -lprintf_flt")
set(CMCU      "-mmcu=${PROCESSOR}")

set(CFLAGS   "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CWARN} ${CSTANDARD} ${CTUNING}")
set(CXXFLAGS "${CMCU} ${CDEBUG} ${CDEFS} ${COPT} ${CTUNING}")

# program names
set(CPP   ${PREFIX}g++)
set(CC     ${PREFIX}gcc)
set(STRIP ${PREFIX}strip)
set(OBJCOPY  ${PREFIX}objcopy)
set(OBJDUMP  ${PREFIX}objdump)
set(SIZE  ${PREFIX}size)


# Sets the compiler
# Needs to come before the project function
set(CMAKE_SYSTEM_NAME  Generic)
set(CMAKE_CXX_COMPILER ${CPP})
set(CMAKE_C_COMPILER   ${CC})
set(CMAKE_ASM_COMPILER ${CC})

set(CMAKE_C_FLAGS   "${CFLAGS}")
set(CMAKE_CXX_FLAGS "${CXXFLAGS}")
set(CMAKE_ASM_FLAGS   "${CFLAGS}")

project(Dice C ASM)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

set(INC_FILES src/main.c
)

add_executable(${PROJECT_NAME} ${INC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}.elf")

target_include_directories(${PROJECT_NAME} PUBLIC "src/peripheral")
target_include_directories(${PROJECT_NAME} PUBLIC "src/shared")

target_link_options(${PROJECT_NAME} PRIVATE
    -Wl,--gc-sections
    -Wl,--relax
)

# Compiling targets
add_custom_target(strip ALL     ${STRIP} "${PROJECT_NAME}.elf" DEPENDS ${PROJECT_NAME})
add_custom_target(hex   ALL     ${OBJCOPY} -R .eeprom -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.hex" && ${SIZE} --format=berkeley ${PROJECT_NAME}.elf DEPENDS strip)
add_custom_target(eeprom        ${OBJCOPY} -j .eeprom --change-section-lma .eeprom=0 -O ihex "${PROJECT_NAME}.elf" "${PROJECT_NAME}.eeprom" DEPENDS strip)

add_custom_target(flash avrdude -c usbasp -p ${MCU} -U flash:w:${PROJECT_NAME}.hex DEPENDS hex)
add_custom_target(fuse avrdude -c usbasp -p ${MCU} -U lfuse:w:0xC1:m && avrdude -c usbasp -p ${MCU} -U hfuse:w:0x99:m && avrdude -c usbasp -p ${MCU} -U lock:w:0xFF:m)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${PROJECT_NAME}.hex;${PROJECT_NAME}.eeprom;${PROJECT_NAME}.lst")
