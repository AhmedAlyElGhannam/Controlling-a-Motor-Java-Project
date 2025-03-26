# find_package("gcc-avr" REQUIRED)

# to make sure CMake knows that the target will not be executed on this machine
set(CMAKE_SYSTEM_NAME Generic)

# setting target architecture to AVR
set(CMAKE_SYSTEM_PROCESSOR avr)

# C version
set(CMAKE_C_STANDARD 99)

# avr- must be part of path environment
set(TOOLCHAIN_PREFIX "/usr/bin/avr-")

# defining toolchain bin tools
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)