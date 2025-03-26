#ifndef MEXTI_REGISTERS_H
#define MEXTI_REGISTERS_H

typedef struct 
{
    volatile uint8_t MCUCSR; 
    volatile uint8_t MCUCR; 
    const uint8_t reserved[4]; 
    volatile uint8_t GIFR; 
    volatile uint8_t GICR; 
} MEXTI_strRegisters_t;

#define MEXTI_BASE_ADDRESS ((volatile void*)(0x54))

#endif