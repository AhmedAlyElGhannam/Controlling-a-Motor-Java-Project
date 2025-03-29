/**
 * @file main.c
 * @brief UART Echo Example for ATmega32
 * 
 * This example demonstrates basic UART communication by echoing received bytes
 * with a preceding '>' character. The implementation uses interrupt-driven
 * UART communication for efficient operation.
 */

#define F_CPU 8000000UL /**< System clock frequency - 8MHz */

#include "std_types.h"
#include "bit_math.h"
#include "usart.h"
#include <avr/delay.h>
#include <avr/interrupt.h>

/**
 * @defgroup Callbacks Callback Functions
 * @{
 */

/**
 * @brief UART receive callback function
 * @param data The received byte
 * 
 * This function is called automatically when a byte is received via UART.
 * It echoes the received byte with a preceding '>' character.
 * 
 * @note This function executes in interrupt context - keep processing minimal
 */
void echoReceivedByte(u8 data)
{
    UART_SendByte('>');      /**< Send prompt character */
    UART_SendByte(data);     /**< Echo the received byte */
}

/** @} */ // end of Callbacks group

/**
 * @brief Main application entry point
 * @return int Not used in embedded environment (infinite loop)
 * 
 * Initializes UART with specified configuration and enters an infinite loop
 * while interrupt-driven UART handling operates in the background.
 */
int main(void)
{
    /**
     * @brief UART configuration structure
     * 
     * Configured for:
     * - 9600 baud rate
     * - 8 data bits
     * - No parity
     * - 1 stop bit
     * - Asynchronous mode
     * - Normal speed
     * - Interrupt-driven receive with echo callback
     */
    UART_Config_t uartConfig = {
        .baudRate = UART_BAUD_9600,     /**< Standard 9600 baud rate */
        .parity = UART_PARITY_NONE,     /**< No parity checking */
        .stopBits = UART_STOP_BITS_1,   /**< 1 stop bit */
        .dataBits = UART_DATA_BITS_8,   /**< 8 data bits */
        .mode = UART_MODE_ASYNC,        /**< Asynchronous operation */
        .speed = UART_SPEED_NORMAL,     /**< Normal speed mode */
        .rxCallback = echoReceivedByte   /**< Receive callback function */
    };

    /* Initialize UART with specified configuration */
    UART_Init(&uartConfig);
    
    /* Brief delay for UART stabilization */
    _delay_ms(100);
    
    /* Enable global interrupts */
    sei();

    /**
     * @brief Main application loop
     * 
     * While UART operations are handled via interrupts,
     * the main loop remains available for other tasks.
     */
    while (1)
    {
        /* Application tasks can be added here */
        /* Current implementation keeps CPU in low-power state */
    }

    /* Never reached */
    return 0;
}
