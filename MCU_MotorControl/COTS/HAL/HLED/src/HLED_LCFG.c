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
#include "HLED.h"
#include "HLED_LCFG.h"

/* array that stores LED configuration and sets it at runtime */
HLED_structLEDConfig_t Global_HLED_structLEDConfigArr[NUM_OF_LEDS] =
{
    [HLED_INVALID_ID] = 
    {
        .portNum = HAL_PORTA,
        .pinNum = HAL_PIN0,
        .connection = HLED_FORWARD
    },
    [HLED_SPEED_REVERSE] = 
    {
        .portNum = HAL_PORTA,
        .pinNum = HAL_PIN1,
        .connection = HLED_FORWARD
    },
    [HLED_TIMEOUT] = 
    {
        .portNum = HAL_PORTA,
        .pinNum = HAL_PIN2,
        .connection = HLED_FORWARD
    },
    [HLED_RECEPTION_SUCCESSFUL] = 
    {
        .portNum = HAL_PORTA,
        .pinNum = HAL_PIN3,
        .connection = HLED_FORWARD
    },
    [HLED_SUCCESSFUL_TRANSACTION] = 
    {
        .portNum = HAL_PORTA,
        .pinNum = HAL_PIN4,
        .connection = HLED_FORWARD
    }
};

