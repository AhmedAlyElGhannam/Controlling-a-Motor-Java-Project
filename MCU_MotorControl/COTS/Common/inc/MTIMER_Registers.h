#ifndef MTIMER_REGISTERS_H
#define MTIMER_REGISTERS_H


/*
    TCCR0
    TCNT0
    OCR0
    TIMSK
    TIFR
*/

typedef struct 
{
    volatile uint8_t OCR2;
    volatile uint8_t TCNT2;
    volatile uint8_t TCCR2;
    volatile uint8_t ICR1L;
    volatile uint8_t ICR1H;
    volatile uint8_t OCR1BL;
    volatile uint8_t OCR1BH;
    volatile uint8_t OCR1AL;
    volatile uint8_t OCR1AH;
    volatile uint8_t TCNT1L;
    volatile uint8_t TCNT1H;
    volatile uint8_t TCCR1B;
    volatile uint8_t TCCR1A;
    const    uint8_t reserved[2];
    volatile uint8_t TCNT0;
    volatile uint8_t TCCR0;
    const    uint8_t reserved2[4];
    volatile uint8_t TIFR;
    volatile uint8_t TIMSK;
    const    uint8_t reserved3[2];
    volatile uint8_t OCR0;
} MTIMER_strRegisters_t;

#define MTIMER_BASE_ADDRESS ((volatile void*)(0x43))

#endif