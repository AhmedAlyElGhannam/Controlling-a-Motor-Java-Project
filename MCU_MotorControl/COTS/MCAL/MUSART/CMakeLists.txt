cmake_minimum_required(VERSION 3.22) # Adjust version as needed
project(MUSART) # Project name
enable_language(C) # used language

# Set AVR-specific compiler and flags
# set(CMAKE_C_COMPILER /usr/bin/avr-gcc) # no need for this line since toolchain script defines it

# set(CMAKE_C_FLAGS "-mmcu=atmega32 -Os") # also no need for that

# Source files (better to list explicitly if few files)
set(SRC_FILES 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/usart.c
)

# Create static library
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

# Set the include directories for the library
# This is where the header files are located
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/inc
)

# compiler options for library
target_compile_options( ${PROJECT_NAME}
    PRIVATE
    -mmcu=${COMPILE_MCU_NAME}
	-DF_CPU=${F_CPU}
	-funsigned-char -funsigned-bitfields -DDEBUG
    -MD -MP -MF "$(@:%.o=%.d)" -MT "$(@:%.o=%.d)" -MT "$(@:%.o=%.o)"
	-Og -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -g2 -Wall
)

# linker options for library
target_link_options( ${PROJECT_NAME}
	PRIVATE
	-Wl,--gc-sections
)
