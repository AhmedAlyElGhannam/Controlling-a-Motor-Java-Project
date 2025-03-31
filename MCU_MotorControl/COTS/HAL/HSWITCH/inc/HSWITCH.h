#ifndef HSWITCH_H
#define HSWITCH_H

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

/* enum for switch action */
typedef enum 
{
	HSWITCH_PRESSED = 0x01,
	HSWITCH_RELEASED = 0x00,
} HSWITCH_enuSwitchAction_t;


/* enum for switch error status */
typedef enum
{
	HSWITCH_OK = 0x00,
    HSWITCH_INVALID_SWITCH_NAME,
    HSWITCH_NULL_PTR,
    HSWITCH_INVALID_SWITCH_STATE,
} HSWITCH_enuErrorStatus_t;

/* enum for switch connections */
typedef enum 
{
    HSWITCH_INTERNAL_PULLUP,
    HSWITCH_EXTERNAL_PULLUP,
    HSWITCH_EXTERNAL_PULLDOWN,
} HSWITCH_enuSwitchConnection_t;

/* enum for defined switch names [DEFINED BY USER] */
/* always start switch name from 0x00 */
typedef enum
{
    HSWITCH_RESET = 0x00,
    HSWITCH_LED_CONTROL = 0x01
} HSWITCH_enuSwitchName_t;


/*
 * @brief Initializes all switches as per the configuration defined in the array located in LCFG.c
 *                   
 * @param None
 *				
 * @return None  
 */
void HSWITCH_voidInit(void);


/*
 * @brief Reads the state of a switch and stores it in a passed address.
 *                   
 * @param (in) Copy_uint8SwitchName -> Switch name as defined by the user in HSWITCH_enuSwitchName_t enum
 * 
 * @param (out) Add_uint8PtrSwitchState -> Pointer to the address at which the state of the switch will be stored
 *				
 * @return HSWITCH_OK || HSWITCH_INVALID_SWITCH_NAME || HSWITCH_NULL_PTR || HSWITCH_INVALID_SWITCH_STATE
 */
HSWITCH_enuErrorStatus_t HSWITCH_enuGetSwitchValue(uint8_t Copy_uint8SwitchName, uint8_t* Add_uint8PtrSwitchState);

#endif