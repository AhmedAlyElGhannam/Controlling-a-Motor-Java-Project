#ifndef TIMER0_PRIVATE_H
#define TIMER0_PRIVATE_H

#define TIMER0_BASE_ADDRESS 0x53

// Timer0 register structure
#define TCCR0               (*((volatile u8*)0x53))
#define TCNT0               (*((volatile u8*)0x52))
#define OCR0                (*((volatile u8*)0x5C))
#define TIMSK               (*((volatile u8*)0x59))
#define TIFR                (*((volatile u8*)0x58))

// Timer0 Control Register (TCCR0) bits
#define FOC0   7 // Force Output Compare
#define WGM00  6 // Waveform Generation Mode bit 0
#define COM01  5 // Compare Match Output Mode bit 1
#define COM00  4 // Compare Match Output Mode bit 0
#define WGM01  3 // Waveform Generation Mode bit 1
#define CS02   2 // Clock Select bit 2
#define CS01   1 // Clock Select bit 1
#define CS00   0 // Clock Select bit 0

// Timer0 Interrupt Mask Register (TIMSK) bits
#define OCIE0  1 // Output Compare Match Interrupt Enable
#define TOIE0  0 // Overflow Interrupt Enable

// Timer0 Interrupt Flag Register (TIFR) bits
#define OCF0   1 // Output Compare Flag
#define TOV0   0 // Overflow Flag

#endif
