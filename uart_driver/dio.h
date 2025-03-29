/**
 * @file dio.h
 * @brief Digital Input/Output (DIO) Driver for AVR Microcontrollers
 * 
 * This header provides an interface for configuring and controlling GPIO pins
 * on AVR microcontrollers (ATmega32). It includes pin/port operations and
 * error handling mechanisms.
 */

#ifndef DIO_H_
#define DIO_H_

/* Include standard types */
#include "std_types.h"

/**
 * @defgroup DIO_Enumerations DIO Enumerations
 * @{
 */

/**
 * @brief Error status enumeration for DIO operations
 */
typedef enum
{
    MDIO_OK,                        /**< Operation completed successfully */
    MDIO_INVALID_PARAM,             /**< Invalid parameter provided */
    MDIO_NOK,                       /**< Operation failed */
    MDIO_NULL_PTR                   /**< Null pointer encountered */
} MDIO_enuErrorStatus_t;

/**
 * @brief Port number enumeration
 */
typedef enum
{
    MDIO_PORTA,                     /**< Port A */
    MDIO_PORTB,                     /**< Port B */
    MDIO_PORTC,                     /**< Port C */
    MDIO_PORTD                      /**< Port D */
} MDIO_enuPortNum_t;

/**
 * @brief Pin number enumeration
 */
typedef enum
{
    MDIO_PIN0,                      /**< Pin 0 */
    MDIO_PIN1,                      /**< Pin 1 */
    MDIO_PIN2,                      /**< Pin 2 */
    MDIO_PIN3,                      /**< Pin 3 */
    MDIO_PIN4,                      /**< Pin 4 */
    MDIO_PIN5,                      /**< Pin 5 */
    MDIO_PIN6,                      /**< Pin 6 */
    MDIO_PIN7                       /**< Pin 7 */
} MDIO_enuPinNum_t;

/**
 * @brief Pin configuration enumeration
 */
typedef enum
{
    MDIO_INPUT,                     /**< Input pin (tri-state) */
    MDIO_OUTPUT,                    /**< Output pin */
    MDIO_INPUT_PULLUP               /**< Input with internal pull-up resistor */
} MDIO_enuPinConfiguration_t;

/**
 * @brief Port configuration enumeration
 */
typedef enum
{
    MDIO_PORT_INPUT = 0x00,                             /**< Entire port as input */
    MDIO_PORT_OUTPUT = 0xFF,                            /**< Entire port as output */
    MDIO_PORT_HIGH_NIBBLE_INPUP_PULL_UP_LOW_NIBBLE_OUTPUT = 0xF0,  /**< High nibble input with pull-up, low nibble output */
    MDIO_PORT_LOW_NIBBLE_INPUT_PULL_UP_HIGH_NIBBLE_OUTPUT = 0x0F   /**< Low nibble input with pull-up, high nibble output */
} MDIO_enuPortConfiguration_t;

/** @} */ // end of DIO_Enumerations group

/**
 * @defgroup DIO_Constants DIO Constants
 * @{
 */
#define MDIO_HIGH 1      /**< Logic high level */
#define MDIO_LOW  0      /**< Logic low level */
/** @} */

/**
 * @defgroup DIO_Registers DIO Register Mapping
 * @brief Hardware register definitions for AVR ports
 * @{
 */

/* PORT A Registers */
#define PORTA_REG (*((volatile u8 *)0x3B))  /**< PORT A Data Register */
#define DDRA_REG  (*((volatile u8 *)0x3A))  /**< PORT A Data Direction Register */
#define PINA_REG  (*((volatile u8 *)0x39))  /**< PORT A Input Pins Register */

/* PORT B Registers */
#define PORTB_REG (*((volatile u8 *)0x38))  /**< PORT B Data Register */
#define DDRB_REG  (*((volatile u8 *)0x37))  /**< PORT B Data Direction Register */
#define PINB_REG  (*((volatile u8 *)0x36))  /**< PORT B Input Pins Register */

/* PORT C Registers */
#define PORTC_REG (*((volatile u8 *)0x35))  /**< PORT C Data Register */
#define DDRC_REG  (*((volatile u8 *)0x34))  /**< PORT C Data Direction Register */
#define PINC_REG  (*((volatile u8 *)0x33))  /**< PORT C Input Pins Register */

/* PORT D Registers */
#define PORTD_REG (*((volatile u8 *)0x32))  /**< PORT D Data Register */
#define DDRD_REG  (*((volatile u8 *)0x31))  /**< PORT D Data Direction Register */
#define PIND_REG  (*((volatile u8 *)0x30))  /**< PORT D Input Pins Register */

/** @} */ // end of DIO_Registers group

/**
 * @defgroup DIO_Functions DIO Functions
 * @{
 */

/**
 * @brief Set the value of a specific pin (high, low, or pull-up)
 * @param Copy_enuPortNum Port number (MDIO_PORTA, MDIO_PORTB, etc.)
 * @param Copy_enuPinNum Pin number (MDIO_PIN0, MDIO_PIN1, etc.)
 * @param Copy_enuPinConfig Pin configuration (MDIO_INPUT, MDIO_OUTPUT, MDIO_INPUT_PULLUP)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port or pin number
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, 
                                         MDIO_enuPinNum_t Copy_enuPinNum, 
                                         u8 Copy_enuPinConfig);

/**
 * @brief Configure the direction of a specific pin (input, output)
 * @param Copy_enuPortNum Port number (MDIO_PORTA, MDIO_PORTB, etc.)
 * @param Copy_enuPinNum Pin number (MDIO_PIN0, MDIO_PIN1, etc.)
 * @param Copy_u8PinDir Pin direction (MDIO_INPUT, MDIO_OUTPUT)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port or pin number
 */
MDIO_enuErrorStatus_t MDIO_enuSetPinConfiguration(MDIO_enuPortNum_t Copy_enuPortNum, 
                                                MDIO_enuPinNum_t Copy_enuPinNum, 
                                                u8 Copy_u8PinDir);

/**
 * @brief Set the value of an entire port (high or low for all pins)
 * @param Copy_enuPortNum Port number (MDIO_PORTA, MDIO_PORTB, etc.)
 * @param Copy_u8PortValue Value to set (0x00-0xFF)
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port number
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortValue(MDIO_enuPortNum_t Copy_enuPortNum, 
                                         u8 Copy_u8PortValue);

/**
 * @brief Configure the direction of an entire port (input, output)
 * @param Copy_enuPortNum Port number (MDIO_PORTA, MDIO_PORTB, etc.)
 * @param Copy_enuPortConfig Port configuration
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port number or configuration
 */
MDIO_enuErrorStatus_t MDIO_enuSetPortDir(MDIO_enuPortNum_t Copy_enuPortNum, 
                                       MDIO_enuPortConfiguration_t Copy_enuPortConfiguration);

/**
 * @brief Get the current value of a specific pin
 * @param Copy_enuPortNum Port number (MDIO_PORTA, MDIO_PORTB, etc.)
 * @param Copy_enuPinNum Pin number (MDIO_PIN0, MDIO_PIN1, etc.)
 * @param Add_pu8PinValue Pointer to store the pin value
 * @return MDIO_enuErrorStatus_t Operation status
 * @retval MDIO_OK Operation successful
 * @retval MDIO_INVALID_PARAM Invalid port or pin number
 * @retval MDIO_NULL_PTR Null pointer provided
 */
MDIO_enuErrorStatus_t MDIO_enuGetPinValue(MDIO_enuPortNum_t Copy_enuPortNum, 
                                        MDIO_enuPinNum_t Copy_enuPinNum, 
                                        u8 *Add_pu8PinValue);

/** @} */ // end of DIO_Functions group

#endif /* DIO_H_ */
