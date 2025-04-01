cmake_minimum_required(VERSION 3.22) # Adjust version as needed
project(MUSART) # Project name

# Set AVR-specific compiler and flags
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)

set(CMAKE_C_FLAGS "-mmcu=atmega32 -Os")

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

