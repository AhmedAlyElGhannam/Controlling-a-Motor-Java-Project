#ifndef MPORT_H
#define MPORT_H

/* enum for error status */
typedef enum 
{
    MPORT_OK = 0, 
    MPORT_ERROR_INVALID_PIN, 
    MPORT_NOK, 
    MPORT_ERROR_INVALID_MODE, 
    MPORT_ERROR_INVALID_DIRECTION,
    MPORT_ERROR_UNCHANGEABLE_MODE,
} MPORT_enuErrorStatus_t;

/* enum for port pins */
typedef enum
{
    MPORT_PIN_A0 = 0x00,
    MPORT_PIN_A1 = 0x01,
    MPORT_PIN_A2 = 0x02,
    MPORT_PIN_A3 = 0x03,
    MPORT_PIN_A4 = 0x04,
    MPORT_PIN_A5 = 0x05,
    MPORT_PIN_A6 = 0x06,
    MPORT_PIN_A7 = 0x07,
    MPORT_PIN_B0 = 0x10,
    MPORT_PIN_B1 = 0x11,
    MPORT_PIN_B2 = 0x12,
    MPORT_PIN_B3 = 0x13,
    MPORT_PIN_B4 = 0x14,
    MPORT_PIN_B5 = 0x15,
    MPORT_PIN_B6 = 0x16,
    MPORT_PIN_B7 = 0x17,
    MPORT_PIN_C0 = 0x20,
    MPORT_PIN_C1 = 0x21,
    MPORT_PIN_C2 = 0x22,
    MPORT_PIN_C3 = 0x23,
    MPORT_PIN_C4 = 0x24,
    MPORT_PIN_C5 = 0x25,
    MPORT_PIN_C6 = 0x26,
    MPORT_PIN_C7 = 0x27,
    MPORT_PIN_D0 = 0x30,
    MPORT_PIN_D1 = 0x31,
    MPORT_PIN_D2 = 0x32,
    MPORT_PIN_D3 = 0x33,
    MPORT_PIN_D4 = 0x34,
    MPORT_PIN_D5 = 0x35,
    MPORT_PIN_D6 = 0x36,
    MPORT_PIN_D7 = 0x37,
} MPORT_enuPortPin_t;

/* enum for port pin directions */
typedef enum
{
    MPORT_PORT_PIN_INPUT = 0x00,
    MPORT_PORT_PIN_OUTPUT = 0x01,
} MPORT_enuPortPinDir_t;

/* enum for port pin modes */
typedef enum
{
    MPORT_PIN_MODE_INPUT_PULLUP,
    MPORT_PIN_MODE_INPUT_PULLDOWN,
    MPORT_PIN_MODE_UART,
    MPORT_PIN_MODE_NONE,
} MPORT_enuPortPinMode_t;


/*
 * @brief	Initializes all pins direction and modes as specified in array defined in LCFG.c
 *                   
 * @param	None
 *				
 * @return  None   
 */
void MPORT_voidInit(void);

/*
 * @brief	Set pin data direction
 *                   
 * @param	Copy_enuPortPinNum 	-> 	MPORT_PIN_A0 to MPORT_PIN_D7 (specified in MPORT.h) 
 * @param   Copy_enuPortPinDir	->	MPORT_PORT_PIN_INPUT, MPORT_PORT_PIN_OUTPUT
 *				
 * @return  MPORT_OK, MPORT_ERROR_INVALID_PIN,MPORT_ERROR_INVALID_DIRECTION
 */
MPORT_enuErrorStatus_t MPORT_enuSetPinDirection(MPORT_enuPortPin_t Copy_enuPortPinNum,  MPORT_enuPortPinDir_t  Copy_enuPortPinDir);

/*
 * @brief	Set pin mode
 *                   
 * @param	Copy_enuPortPinNum 	-> 	MPORT_PIN_A0 to MPORT_PIN_D7 (specified in MPORT.h) 
 * @param   Copy_enuPortPinMode	->	MPORT_PIN_MODE_INPUT_PULLUP, MPORT_PIN_MODE_INPUT_PULLDOWN, MPORT_PIN_MODE_UART
 *				
 * @return  MPORT_OK, MPORT_ERROR_INVALID_PIN, MPORT_ERROR_INVALID_MODE, MPORT_ERROR_UNCHANGEABLE_MODE
 */
MPORT_enuErrorStatus_t MPORT_enuSetPinMode(MPORT_enuPortPin_t Copy_enuPortPinNum, MPORT_enuPortPinMode_t Copy_enuPortPinMode);

#endif 