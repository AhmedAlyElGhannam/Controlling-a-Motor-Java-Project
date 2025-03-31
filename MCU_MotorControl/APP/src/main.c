#include "std_types.h"
#include "bit_math.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "MPORT.h"
#include "MDIO.h"
#include "HLED.h"
#include "MGIE.h"
#include "MEXTI.h"
#include "MTIMER.h"
#include "HSCHEDULER.h"
#include "usart.h"


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
void echoReceivedByte(uint8_t data)
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
void main(void)
{
	cli();
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

	 while (true) {}
}