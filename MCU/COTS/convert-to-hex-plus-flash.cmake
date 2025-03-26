macro(convert_to_hex_and_flash PROJECT_NAME TARGET_MCU PROGRAMMER)
   # adding a custom target/build rule for creating .hex file
    add_custom_target(convert-elf-to-hex 
    ALL
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating .hex file from .elf file"
    )

    # adding a custom target/build rule for burning .hex file on mcu using avrdude
    add_custom_target(flash
    DEPENDS convert-elf-to-hex
    COMMAND avrdude -c ${PROGRAMMER} -p ${TARGET_MCU} -U flash:w:${PROJECT_NAME}.hex:i -B 1
    COMMENT "Burning .hex file into mcu flash"
    )    
endmacro()

