# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# toolchain settings
include(${CMAKE_CURRENT_SOURCE_DIR}/../TOOLCHAIN/avr-gcc.cmake)

# project name + defining used languages 
project(AVR_PROJECT)
enable_language(C)

# set C && Cpp versions
set(CMAKE_C_STANDARD 99)

# avr mcu is ATMEGA32 clocked @16MHz
set(COMPILE_MCU_NAME atmega32) 	# for compiler
set(FLASH_MCU_NAME m32)    	# for avrdude
set(F_CPU 8000000UL)	# for frequency

# programmer type 
set(PROGRAMMER usbasp) 

# collect all .c files
file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# defining a list of MCAL libraries that will be used to build HAL libraries
set(MCAL_LIB
	MGIE
	MDIO
	MPORT	 
	MEXTI
	MTIMER
)

# looping over all libs to build them
foreach(mcal_lib ${MCAL_LIB})
	add_subdirectory(
	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/MCAL/${mcal_lib}/"
	    "${CMAKE_BINARY_DIR}/${mcal_lib}/"
	)
endforeach()

# defining a list of HAL libraries that app depends on
set(HAL_LIB
	HLED
	HSWITCH
	H7SEGMENT
	HLCD
	HKEYPAD
	HSCHEDULER
)

# looping over all HAL libs to build them
foreach(hal_lib ${HAL_LIB})
	add_subdirectory(
	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/HAL/${hal_lib}"
	    "${CMAKE_BINARY_DIR}/${hal_lib}"
	)
endforeach()

# setting default compiler for C in this CMake file
# set(CMAKE_C_COMPILER "/usr/bin/avr-gcc")

# include ,cmake script for converting .elf to .hex and flashing
include(${CMAKE_CURRENT_SOURCE_DIR}/../COTS/convert-to-hex-plus-flash.cmake)

# include ,cmake script for launching avrsimul && gdb for debugging
include(${CMAKE_CURRENT_SOURCE_DIR}/../COTS/avr-simul-gdb-debug.cmake)

# compiling and getting .elf file
add_executable(${PROJECT_NAME}.elf "${${PROJECT_NAME}_SRC_FILES}") # main target

target_compile_options( ${PROJECT_NAME}.elf
    PRIVATE
    -mmcu=${COMPILE_MCU_NAME}
	-DF_CPU=${F_CPU}
	-funsigned-char -funsigned-bitfields -DDEBUG
	-MD -MP -MF "$(@:%.o=%.d)" -MT "$(@:%.o=%.d)" -MT "$(@:%.o=%.o)"
	-Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall
	-I${AVR_INCLUDE_DIR}
)

# linker options for target
target_link_options( ${PROJECT_NAME}.elf
	PRIVATE
	-Wl,--gc-sections
	-mmcu=${COMPILE_MCU_NAME}
)

# MPORT library is the only MCAL lib that can be used in app layer + link all HAL libs
target_link_libraries(${PROJECT_NAME}.elf PRIVATE ${MCAL_LIB} ${HAL_LIB})

# calling macro for converting .elf into .hex and flashing it on specified mcu
convert_to_hex_and_flash(${PROJECT_NAME} ${FLASH_MCU_NAME} ${PROGRAMMER})

# calling a macro that creates a custom target for launching avrsimul && gdb for debugging
avr_simul_gdb_debug(${PROJECT_NAME})

# modify the clean build rule to delete the .hex file
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex)
