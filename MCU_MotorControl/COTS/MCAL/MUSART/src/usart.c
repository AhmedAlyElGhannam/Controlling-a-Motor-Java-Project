/**
 * @file usart.c
 * @brief USART/UART driver implementation for ATmega32
 * @author Omar Moamed Mostafa
 * @date 2023
 * @copyright Your License (if any)
 *
 * This file contains the implementation of USART/UART communication functions
 * for the ATmega32 microcontroller, including initialization, data transmission,
 * reception, and interrupt handling.
 */

#include "std_types.h"
#include "bit_math.h"
#include "usart.h"
#include <avr/interrupt.h>

/* Static function prototypes */
static void UART_SetBaudRate(UART_BaudRate_t baudRate, UART_Speed_t speed);
static void UART_SetFrameFormat(UART_DataBits_t dataBits, UART_Parity_t parity, UART_StopBits_t stopBits);
static void UART_SetMode(UART_Mode_t mode);

/**
 * @var userRxCallback
 * @brief Callback function pointer for receive interrupts
 * @note This is volatile as it's modified outside ISR context
 */
static volatile UART_RxCallback_t userRxCallback = NULL;

/**
 * @brief Sets the receive callback function
 * @param callback Function pointer to be called when data is received
 */
void UART_SetRxCallback(UART_RxCallback_t callback)
{
	userRxCallback = callback;
}

/**
 * @brief Enables UART receive interrupt
 */
void UART_EnableRxInterrupt(void)
{
	UCSRB |= (1 << RXCIE); // Enable RX Complete Interrupt
}

/**
 * @brief Disables UART receive interrupt
 */
void UART_DisableRxInterrupt(void)
{
	UCSRB &= ~(1 << RXCIE); // Disable RX Complete Interrupt
}

/**
 * @brief Initializes the UART module with specified configuration
 * @param config Pointer to UART configuration structure
 *
 * Initialization sequence:
 * 1. Sets baud rate (most timing-critical)
 * 2. Configures frame format (data bits, parity, stop bits)
 * 3. Sets operation mode (async/sync)
 * 4. Enables transmitter and receiver
 */
void UART_Init(const UART_Config_t *config)
{
	/* Set baud rate first - most critical */
	UART_SetBaudRate(config->baudRate, config->speed);

	/* Configure frame format */
	UART_SetFrameFormat(config->dataBits, config->parity, config->stopBits);

	/* Set operation mode */
	UART_SetMode(config->mode);

	/* Set callback before enabling interrupts */
	userRxCallback = config->rxCallback;

	/* Enable UART and interrupts in one operation */
	UCSRB = (1 << RXCIE) | (1 << TXEN) | (1 << RXEN);

	/* Debug output */
	// UART_SendString("UART Ready - Echo Test\r\n");
	// if (userRxCallback == NULL)
	// {
	// 	UART_SendString("WARNING: No callback set!\r\n");
	// }
}

/**
 * @brief UART Receive Complete Interrupt Service Routine
 *
 * Handles incoming data by either:
 * - Calling the user callback if set
 * - Sending '?' character if no callback is set (debug)
 */
ISR(USART_RXC_vect)
{
	uint8_t receivedData = UDR;
	if (userRxCallback != NULL)
	{
		userRxCallback(receivedData);
	}
	else
	{
		UART_SendByte('?'); // Debug: shows callback wasn't set
	}
}

/**
 * @brief Sends a single byte via UART
 * @param data The byte to be transmitted
 *
 * Blocks until transmit buffer is empty
 */
void UART_SendByte(uint8_t data)
{
	/* Wait for empty transmit buffer */
	while (!(UCSRA & (1 << UDRE)))
		;

	/* Put data into buffer */
	UDR = data;
}

/**
 * @brief Receives a single byte via UART
 * @return The received byte
 *
 * Blocks until data is received
 */
uint8_t UART_ReceiveByte(void)
{
	/* Wait for data to be received */
	while (!(UCSRA & (1 << RXC)))
		;

	/* Get and return received data */
	return UDR;
}

/**
 * @brief Sends a null-terminated string via UART
 * @param str Pointer to the string to be transmitted
 */
void UART_SendString(const char *str)
{
	while (*str)
	{
		UART_SendByte(*str++);
	}
}

/**
 * @brief Receives a string until newline or buffer full
 * @param buf Buffer to store received string
 * @param maxLen Maximum number of characters to receive (including null terminator)
 *
 * Reception stops when either:
 * - Carriage return or newline is received
 * - maxLen-1 characters are received
 * The string is always null-terminated
 */
void UART_ReceiveString(char *buf, uint16_t maxLen)
{
	uint16_t i = 0;
	char c;

	do
	{
		c = UART_ReceiveByte();
		if (i < maxLen - 1)
		{
			buf[i++] = c;
		}
	} while (c != '\r' && c != '\n' && i < maxLen);

	buf[i] = '\0';
}

/**
 * @brief Sets the UART baud rate
 * @param baudRate Desired baud rate
 * @param speed Normal or double speed mode
 *
 * Calculates and sets the UBRR value based on:
 * - 8MHz clock (adjust if using different clock)
 * - Selected speed mode
 */
static void UART_SetBaudRate(UART_BaudRate_t baudRate, UART_Speed_t speed)
{
	uint16_t ubrr;

	if (speed == UART_SPEED_NORMAL)
	{
		// Changed from 16MHz to 8MHz calculation
		ubrr = (8000000UL / 16 / baudRate) - 1;
		UCSRA &= ~(1 << U2X);
	}
	else
	{
		// Double speed mode for 8MHz
		ubrr = (8000000UL / 8 / baudRate) - 1;
		UCSRA |= (1 << U2X);
	}

	UBRRH = (uint8_t)(ubrr >> 8);
	UBRRL = (uint8_t)ubrr;
}

/**
 * @brief Configures UART frame format
 * @param dataBits Number of data bits (5-9)
 * @param parity Parity mode (none, even, odd)
 * @param stopBits Number of stop bits (1 or 2)
 *
 * Sets UCSRC register with proper configuration:
 * - Data bits (UCSZ)
 * - Parity (UPM)
 * - Stop bits (USBS)
 */
static void UART_SetFrameFormat(UART_DataBits_t dataBits, UART_Parity_t parity, UART_StopBits_t stopBits)
{
	uint8_t config = (1 << URSEL); // Select UCSRC

	/* Data bits configuration */
	switch (dataBits)
	{
	case UART_DATA_BITS_5:
		config |= (0 << UCSZ1) | (0 << UCSZ0);
		break;
	case UART_DATA_BITS_6:
		config |= (0 << UCSZ1) | (1 << UCSZ0);
		break;
	case UART_DATA_BITS_7:
		config |= (1 << UCSZ1) | (0 << UCSZ0);
		break;
	case UART_DATA_BITS_8:
		config |= (1 << UCSZ1) | (1 << UCSZ0);
		break;
	case UART_DATA_BITS_9:
		config |= (1 << UCSZ1) | (1 << UCSZ0);
		UCSRB |= (1 << UCSZ2);
		break;
	}

	/* Parity configuration */
	switch (parity)
	{
	case UART_PARITY_NONE:
		config |= (0 << UPM1) | (0 << UPM0);
		break;
	case UART_PARITY_EVEN:
		config |= (1 << UPM1) | (0 << UPM0);
		break;
	case UART_PARITY_ODD:
		config |= (1 << UPM1) | (1 << UPM0);
		break;
	}

	/* Stop bits configuration */
	if (stopBits == UART_STOP_BITS_2)
	{
		config |= (1 << USBS);
	}

	UCSRC = config;
}

/**
 * @brief Sets UART operation mode
 * @param mode Asynchronous or synchronous mode
 */
static void UART_SetMode(UART_Mode_t mode)
{
	if (mode == UART_MODE_SYNC)
	{
		UCSRC |= (1 << UMSEL);
	}
	else
	{
		UCSRC &= ~(1 << UMSEL);
	}
}