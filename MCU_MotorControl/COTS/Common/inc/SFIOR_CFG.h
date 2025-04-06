#ifndef SFIOR_H
#define SFIOR_H

#define SFIOR   (*((volatile uint8_t*)0x50))

// SFIOR = (SFIOR & MODE_MASK) | (MODE << 3)

#endif