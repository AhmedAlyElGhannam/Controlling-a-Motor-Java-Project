#ifndef DIO_PRIVATE_H
#define DIO_PRIVATE_H

#define PORTA_REG (*(volatile unsigned char*)0x3B)
#define PORTB_REG (*(volatile unsigned char*)0x38)
#define PORTC_REG (*(volatile unsigned char*)0x35)
#define PORTD_REG (*(volatile unsigned char*)0x32)

#define DDRA_REG (*(volatile unsigned char*)0x3A)  // Data Direction Register for PORTA
#define DDRB_REG (*(volatile unsigned char*)0x37)  // Data Direction Register for PORTB
#define DDRC_REG (*(volatile unsigned char*)0x34)  // Data Direction Register for PORTC
#define DDRD_REG (*(volatile unsigned char*)0x31)  // Data Direction Register for PORTD

#define PINA_REG (*(volatile unsigned char*)0x39)  // Input Pins for PORTA
#define PINB_REG (*(volatile unsigned char*)0x36)  // Input Pins for PORTB
#define PINC_REG (*(volatile unsigned char*)0x33)  // Input Pins for PORTC
#define PIND_REG (*(volatile unsigned char*)0x30)  // Input Pins for PORTD


#endif //DIO_PRIVATE_H