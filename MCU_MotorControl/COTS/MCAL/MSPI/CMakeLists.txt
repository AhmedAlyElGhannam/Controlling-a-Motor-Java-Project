# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# project name + defining used languages
project(MDIO C) 

# setting default compiler for C && Cpp
set(CMAKE_C_COMPILER /usr/bin/avr-gcc)

# collect library source files
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# collect library header files
file(GLOB LIB_INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
file(GLOB STDLIB_INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/inc/*.h)
file(GLOB COMMON_INC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/inc/*.h)

# combine all header file variables into a single variable
set(INC_FILES ${LIB_INC_FILES} ${STDLIB_INC_FILES} ${COMMON_INC_FILES})

# include directory for all .h files used by MDIO lib
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Common/inc
)

# create a static library
add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${INC_FILES})

# in order to allow main to use MDIO.h & MDIO_cfg -> make it interface
target_include_directories(${PROJECT_NAME} INTERFACE ${PROJECT_SOURCE_DIR}/inc)

# std_lib will be public to anyone who wants to use though MDIO
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/../../STD_LIB/inc)

# Common lib aka registers should ABSOLUTELY BE private
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/../../Common/inc)
