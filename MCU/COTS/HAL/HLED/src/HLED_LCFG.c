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
    [HLED_START] = 
    {
        .portNum = PORTD,
        .pinNum = PIN0,
        .connection = HLED_FORWARD
    }
    /*[HLED_ALERT] = 
    {
        .portNum = PORTC,
        .pinNum = PIN0,
        .connection = HLED_FORWARD
    },
    [HLED_STOP] = 
    {
        .portNum = PORTB,
        .pinNum = PIN0,
        .connection = HLED_FORWARD
    }*/
};

