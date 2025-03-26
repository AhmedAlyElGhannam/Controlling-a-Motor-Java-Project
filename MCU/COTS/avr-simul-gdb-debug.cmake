macro(avr_simul_gdb_debug PROJECT_NAME)
    # adding a custom target/build rule for creating .hex file
    add_custom_target(launch-avr-simul
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND simulavr -d atmega32 -f ${PROJECT_NAME}.elf --gdbserver
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Launching AVR simul"
    )

    # adding a custom target/build rule for burning .hex file on mcu using avrdude
    add_custom_target(launch-gdb
    DEPENDS launch-avr-simul
    COMMAND /usr/bin/avr-gdb -q ${PROJECT_NAME} -x ${CMAKE_BINARY_DIR}/../../GDB_CFG/gdb.cfg
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Opening GDB for debugging"
    )    
endmacro()

