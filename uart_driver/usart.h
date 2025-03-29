/**
 * @file usart.h
 * @brief USART/UART driver for ATmega32 microcontroller
 * @author Omar Moamed Mostafa
 * @date 2023
 * @copyright Your License (if any)
 *
 * This header file contains all the definitions, configurations and function
 * prototypes for USART/UART communication on ATmega32 microcontroller.
 */

#ifndef USART_H
#define USART_H

#include <stdint.h>

/**
 * @brief Baud rate options for UART communication
 */
typedef enum
{
    UART_BAUD_2400 = 2400,    ///< 2400 baud
    UART_BAUD_9600 = 9600,    ///< 9600 baud
    UART_BAUD_19200 = 19200,  ///< 19200 baud
    UART_BAUD_38400 = 38400,  ///< 38400 baud
    UART_BAUD_57600 = 57600,  ///< 57600 baud
    UART_BAUD_115200 = 115200 ///< 115200 baud
} UART_BaudRate_t;

/**
 * @brief Parity options for UART communication
 */
typedef enum
{
    UART_PARITY_NONE, ///< No parity
    UART_PARITY_EVEN, ///< Even parity
    UART_PARITY_ODD   ///< Odd parity
} UART_Parity_t;

/**
 * @brief Stop bits options for UART communication
 */
typedef enum
{
    UART_STOP_BITS_1, ///< 1 stop bit
    UART_STOP_BITS_2  ///< 2 stop bits
} UART_StopBits_t;

/**
 * @brief Data bits options for UART communication
 */
typedef enum
{
    UART_DATA_BITS_5, ///< 5 data bits
    UART_DATA_BITS_6, ///< 6 data bits
    UART_DATA_BITS_7, ///< 7 data bits
    UART_DATA_BITS_8, ///< 8 data bits
    UART_DATA_BITS_9  ///< 9 data bits
} UART_DataBits_t;

/**
 * @brief UART operation mode options
 */
typedef enum
{
    UART_MODE_ASYNC, ///< Asynchronous operation
    UART_MODE_SYNC   ///< Synchronous operation
} UART_Mode_t;

/**
 * @brief UART speed options
 */
typedef enum
{
    UART_SPEED_NORMAL, ///< Normal speed
    UART_SPEED_DOUBLE  ///< Double speed
} UART_Speed_t;

/**
 * @brief Callback function type for UART receive
 * @param receivedData The byte received by UART
 */
typedef void (*UART_RxCallback_t)(uint8_t receivedData);

/**
 * @brief UART configuration structure
 *
 * This structure holds all configuration parameters for UART initialization
 */
typedef struct
{
    UART_BaudRate_t baudRate;     ///< Baud rate setting
    UART_Parity_t parity;         ///< Parity setting
    UART_StopBits_t stopBits;     ///< Stop bits setting
    UART_DataBits_t dataBits;     ///< Data bits setting
    UART_Mode_t mode;             ///< Operation mode
    UART_Speed_t speed;           ///< Operation speed
    UART_RxCallback_t rxCallback; ///< Receive callback function pointer
} UART_Config_t;

/* ATmega32 UART Register Definitions */
#define UDR (*((volatile uint8_t *)0x2C))   ///< UART Data Register
#define UCSRA (*((volatile uint8_t *)0x2B)) ///< UART Control and Status Register A
#define UCSRB (*((volatile uint8_t *)0x2A)) ///< UART Control and Status Register B
#define UCSRC (*((volatile uint8_t *)0x40)) ///< UART Control and Status Register C
#define UBRRL (*((volatile uint8_t *)0x29)) ///< Baud Rate Low Register
#define UBRRH (*((volatile uint8_t *)0x40)) ///< Baud Rate High Register

/* UCSRA Bits */
#define RXC 7  ///< Receive Complete flag
#define TXC 6  ///< Transmit Complete flag
#define UDRE 5 ///< Data Register Empty flag
#define FE 4   ///< Frame Error flag
#define DOR 3  ///< Data OverRun flag
#define PE 2   ///< Parity Error flag
#define U2X 1  ///< Double Transmission Speed
#define MPCM 0 ///< Multi-processor Communication Mode

/* UCSRB Bits */
#define RXCIE 7 ///< RX Complete Interrupt Enable
#define TXCIE 6 ///< TX Complete Interrupt Enable
#define UDRIE 5 ///< Data Register Empty Interrupt Enable
#define RXEN 4  ///< Receiver Enable
#define TXEN 3  ///< Transmitter Enable
#define UCSZ2 2 ///< Character Size bit 2
#define RXB8 1  ///< Receive Data Bit 8
#define TXB8 0  ///< Transmit Data Bit 8

/* UCSRC Bits */
#define URSEL 7 ///< Register Select (1 for UCSRC, 0 for UBRRH)
#define UMSEL 6 ///< USART Mode Select
#define UPM1 5  ///< Parity Mode bit 1
#define UPM0 4  ///< Parity Mode bit 0
#define USBS 3  ///< Stop Bit Select
#define UCSZ1 2 ///< Character Size bit 1
#define UCSZ0 1 ///< Character Size bit 0
#define UCPOL 0 ///< Clock Polarity

/* Function Prototypes */

/**
 * @brief Initializes UART with given configuration
 * @param config Pointer to UART configuration structure
 */
void UART_Init(const UART_Config_t *config);

/**
 * @brief Sends a single byte via UART
 * @param data The byte to be sent
 */
void UART_SendByte(uint8_t data);

/**
 * @brief Receives a single byte via UART
 * @return The received byte
 */
uint8_t UART_ReceiveByte(void);

/**
 * @brief Sends a null-terminated string via UART
 * @param str Pointer to the string to be sent
 */
void UART_SendString(const char *str);

/**
 * @brief Receives a string until newline or max length is reached
 * @param buf Buffer to store received string
 * @param maxLen Maximum length of string to receive
 */
void UART_ReceiveString(char *buf, uint16_t maxLen);

/**
 * @brief Sets the receive callback function
 * @param callback Function pointer to the callback
 */
void UART_SetRxCallback(UART_RxCallback_t callback);

/**
 * @brief Enables UART receive interrupt
 */
void UART_EnableRxInterrupt(void);

/**
 * @brief Disables UART receive interrupt
 */
void UART_DisableRxInterrupt(void);

#endif /* USART_H */