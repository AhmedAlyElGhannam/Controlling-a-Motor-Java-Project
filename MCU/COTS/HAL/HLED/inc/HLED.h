#ifndef HLED_H
#define HLED_H

/* enum for port and pin numbers --- common for all HAL so they must be defined once */
#ifndef HAL_PORT_AND_PIN_NAMES
#define HAL_PORT_AND_PIN_NAMES
typedef enum
{
    PORTA = 0x00,
    PORTB,
    PORTC,
    PORTD = 0x03,
} HAL_enuPortName_t;

typedef enum
{
    PIN0 = 0x00,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7 = 0x07,
} HAL_enuPinName_t;
#endif

/* enum for LED value */
typedef enum 
{
	HLED_ON = 0x01,
	HLED_OFF = 0x00,
} HLED_enuLEDValue_t;

/* enum for LED connection */
typedef enum 
{
    HLED_FORWARD = 0x01,
    HLED_REVERSE = 0x00,
} HLED_enuLEDConnection_t;

/* enum for LED error status */
typedef enum
{
	HLED_OK = 0x00,
    HLED_INVALID_LED_NAME,
    HLED_INVALID_LED_VALUE,
} HLED_enuErrorStatus_t;

/* enum for defined LED names [DEFINED BY USER] */
/* always start LED name from 0x00 */
typedef enum
{
    HLED_START = 0x00,
    HLED_ALERT = 0x01,
    HLED_STOP = 0x02,
} HLED_enuLEDName_t;


/*
 * @brief	Initializes all LEDs as per the configuration defined in the array located in LCFG.c
 *                   
 * @param	None
 *				
 * @return  None   
 */
void HLED_voidInit(void);

/*
 * @brief	Sets LED value to ON or OFF
 *                   
 * @param (in) Copy_uint8LEDName -> LED name as defined by the user in HLED_enuLEDName_t enum
 * 
 * @param (in) Copy_uint8LEDValue -> HLED_ON || HLED_OFF
 *				
 * @return  HLED_OK || HLED_INVALID_LED_NAME || HLED_INVALID_LED_VALUE   
 */
HLED_enuErrorStatus_t HLED_uint8SetLEDValue(uint8_t Copy_uint8LEDName, HLED_enuLEDValue_t Copy_uint8LEDValue);

#endif