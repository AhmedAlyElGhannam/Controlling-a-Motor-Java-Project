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
#include "HLED.h"
#include "HLED_LCFG.h"


/* macros for checking function arguments */
#define IS_INVALID_LED_NAME(X)			((((HLED_enuLEDName_t)X) > (NUM_OF_LEDS - 1)) || (((HLED_enuLEDName_t)X) < (FIRST_LED)))
#define IS_INVALID_LED_VALUE(X)			((((HLED_enuLEDValue_t)X) != HLED_ON) && (((HLED_enuLEDValue_t)X) != HLED_OFF))

/* accessing led configuration array defined in LCFG.c file */
extern HLED_structLEDConfig_t Global_HLED_structLEDConfigArr[NUM_OF_LEDS];


/*
 * @brief	Initializes all LEDs as per the configuration defined in the array located in LCFG.c
 *                   
 * @param	None
 *				
 * @return  None   
 */
void HLED_voidInit(void)
{
	/* defining variables for port pin && iterator */
	uint8_t Local_uint8CurrPortPin = 0x00;
	uint8_t Local_uint8Iter;

	for (Local_uint8Iter = FIRST_LED; Local_uint8Iter < NUM_OF_LEDS; Local_uint8Iter++)
	{
		/* extract and combine port && pin numbers into a single value to pass to MPORT function */
		Local_uint8CurrPortPin = SET_HIGH_NIB_TO_VAL(Local_uint8CurrPortPin, Global_HLED_structLEDConfigArr[Local_uint8Iter].portNum) + SET_LOW_NIB_TO_VAL(Local_uint8CurrPortPin, Global_HLED_structLEDConfigArr[Local_uint8Iter].pinNum);

		/* configure LED pin as output */
		MPORT_enuSetPinDirection(Local_uint8CurrPortPin, MPORT_PORT_PIN_OUTPUT);
	}
}

/*
 * @brief	Sets LED value to ON or OFF
 *                   
 * @param (in) Copy_uint8LEDName -> LED name as defined by the user in HLED_enuLEDName_t enum
 * 
 * @param (in) Copy_uint8LEDValue -> HLED_ON || HLED_OFF
 *				
 * @return  HLED_OK || HLED_INVALID_LED_NAME || HLED_INVALID_LED_VALUE   
 */
HLED_enuErrorStatus_t HLED_uint8SetLEDValue(uint8_t Copy_uint8LEDName, HLED_enuLEDValue_t Copy_uint8LEDValue)
{
    /* defining a variable to store return address */
	HLED_enuErrorStatus_t ret_enuStatus = HLED_OK;

	/* defining variable to store the actual state of LED according to connection */
	uint8_t Local_uint8LEDActualState = Copy_uint8LEDValue;

	if (IS_INVALID_LED_NAME(Copy_uint8LEDName))
	{
		/* do not continue if passed LED name is not valid */
		ret_enuStatus = HLED_INVALID_LED_NAME;
	}
	else if (IS_INVALID_LED_VALUE(Copy_uint8LEDValue))
	{
		/* do not continue if passed LED value is not valid */
		ret_enuStatus = HLED_INVALID_LED_VALUE;
	}
	else /* all arguments are valid */
	{
		
		if ((Global_HLED_structLEDConfigArr[Copy_uint8LEDName].connection) == HLED_FORWARD)
		{
			/* ON corresponds to logic HIGH, OFF corresponds to logic LOW */
			Local_uint8LEDActualState = Copy_uint8LEDValue;
		}
		else if ((Global_HLED_structLEDConfigArr[Copy_uint8LEDName].connection) == HLED_REVERSE)
		{
			/* ON corresponds to logic LOW, OFF corresponds to logic HIGH */
			Local_uint8LEDActualState = !Copy_uint8LEDValue;
		}
		else {} /* there is no 3rd connection */

		/* use MDIO's API to set the value of the pin the LED is connected to */
		ret_enuStatus = MDIO_enuSetPinValue(
			Global_HLED_structLEDConfigArr[Copy_uint8LEDName].portNum,
			Global_HLED_structLEDConfigArr[Copy_uint8LEDName].pinNum,
			Local_uint8LEDActualState
		);
	}

	return ret_enuStatus;
}