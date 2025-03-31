#include "bit_math.h"
/* to include right reg .h file during testing */
#ifdef UNIT_TESTING_MODE	
	/* header files that are included only when in testing mode */
	#include <stdint.h>
	#include <stddef.h>
	#include <stdbool.h>
#else
	#include "std_types.h"
#endif
#include "MDIO_PBCFG.h"
#include "MPORT_LCFG.h"
#include "MDIO.h"
#include "MPORT.h"
#include "HSWITCH.h"
#include "HSWITCH_LCFG.h"


/* macros for checking function arguments */
#define IS_INVALID_SWITCH_NAME(X)					((((uint8_t)X) > NUM_OF_SWITCHES) || (((uint8_t)X) < FIRST_SWITCH))
#define IS_INVALID_SWITCH_STATE(X)					((((HLED_enuLEDValue_t)X) != HLED_ON) && (((HLED_enuLEDValue_t)X) != HLED_OFF))
#define IS_CONNECTION_HSWITCH_INTERNAL_PULLUP(X)	(((HSWITCH_enuSwitchConnection_t)X) == HSWITCH_INTERNAL_PULLUP)
#define IS_INVALID_PTR(X)							((X) == NULL)

/* accessing led configuration array defined in LCFG.c file */
extern HSWITCH_structSwitchConfig_t Global_HSWITCH_structSwitchConfigArr[NUM_OF_SWITCHES];


/*
 * @brief Initializes all switches as per the configuration defined in the array located in LCFG.c
 *                   
 * @param None
 *				
 * @return None  
 */
void HSWITCH_voidInit(void)
{
	/* defining variables for port pin && iterator */
	uint8_t Local_uint8CurrPortPin = 0x00;
	uint8_t Local_uint8Iter;

	for (Local_uint8Iter = 0; Local_uint8Iter < NUM_OF_SWITCHES; Local_uint8Iter++)
	{
		/* extract and combine port && pin numbers into a single value to pass to MPORT function */
		Local_uint8CurrPortPin = SET_HIGH_NIB_TO_VAL(Local_uint8CurrPortPin, Global_HSWITCH_structSwitchConfigArr[Local_uint8Iter].portNum) + SET_LOW_NIB_TO_VAL(Local_uint8CurrPortPin, Global_HSWITCH_structSwitchConfigArr[Local_uint8Iter].pinNum);		
		
		/* configure switch pin as input */
		MPORT_enuSetPinDirection(Local_uint8CurrPortPin, MPORT_PORT_PIN_INPUT);
		if (IS_CONNECTION_HSWITCH_INTERNAL_PULLUP(Global_HSWITCH_structSwitchConfigArr[Local_uint8Iter].connection))
		{
			/* enable input pullup if this is the switch's connection */
			MPORT_enuSetPinMode(Local_uint8CurrPortPin, MPORT_PIN_MODE_INPUT_PULLUP);
		}
		else /* EXTERNAL_PULLDOWN || EXTERNAL PULLUP */
		{
			/* disable input pullup if external connection is used */
			MPORT_enuSetPinMode(Local_uint8CurrPortPin, MPORT_PIN_MODE_INPUT_PULLDOWN);
		}
	}
}


/*
 * @brief Reads the state of a switch and stores it in a passed address.
 *                   
 * @param (in) Copy_uint8SwitchName -> Switch name as defined by the user in HSWITCH_enuSwitchName_t enum
 * 
 * @param (out) Add_uint8PtrSwitchState -> Pointer to the address at which the state of the switch will be stored
 *				
 * @return HSWITCH_OK || HSWITCH_INVALID_SWITCH_NAME || HSWITCH_NULL_PTR || HSWITCH_INVALID_SWITCH_STATE
 */
HSWITCH_enuErrorStatus_t HSWITCH_enuGetSwitchValue(uint8_t Copy_uint8SwitchName, uint8_t* Add_uint8PtrSwitchState)
{
    /* defining a variable to store return address */
	HSWITCH_enuErrorStatus_t ret_enuStatus = HSWITCH_OK;

	if (IS_INVALID_SWITCH_NAME(Copy_uint8SwitchName))
	{
		/* do not continue if passed switch name is not valid */
		ret_enuStatus = HSWITCH_INVALID_SWITCH_NAME;
	}
	else if (IS_INVALID_PTR(Add_uint8PtrSwitchState))
	{
		/* do not continue if passed pointer is NULL */
		ret_enuStatus = HSWITCH_NULL_PTR;
	}
	else /* all arguments are valid */
	{
		/* use MDIO's API to read the value of the pin the switch is connected to */
		ret_enuStatus = MDIO_enuGetPinValue(
			Global_HSWITCH_structSwitchConfigArr[Copy_uint8SwitchName].portNum,
			Global_HSWITCH_structSwitchConfigArr[Copy_uint8SwitchName].pinNum,
			Add_uint8PtrSwitchState
		);
	}

	return ret_enuStatus;
}