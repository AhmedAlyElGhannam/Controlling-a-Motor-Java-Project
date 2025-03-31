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
#include "H7SEGMENT.h"
#include "H7SEGMENT_LCFG.h"

/* array that stores LED configuration and sets it at runtime */
H7SEGMENT_struct7SegmentConfig_t Global_H7SEGMENT_struct7SegmentConfigArr[NUM_OF_7SEG] =
{
    [H7SEGMENT_COUNTER_UNITS] = 
    {
        .portNum = PORTA,
        .pinsConnection = 
        {
            [H7SEGMENT_PIN_a] = PIN0,
            [H7SEGMENT_PIN_b] = PIN1,
            [H7SEGMENT_PIN_c] = PIN2,
            [H7SEGMENT_PIN_d] = PIN3,
            [H7SEGMENT_PIN_e] = PIN4,
            [H7SEGMENT_PIN_f] = PIN5,
            [H7SEGMENT_PIN_g] = PIN6,
            [H7SEGMENT_PIN_dot] = PIN7
        },
        .connection = H7SEGMENT_COMMON_CATHODE
    },
    [H7SEGMENT_COUNTER_TENS] = 
    {
        .portNum = PORTB,
        .pinsConnection = 
        {
            [H7SEGMENT_PIN_a] = PIN0,
            [H7SEGMENT_PIN_b] = PIN1,
            [H7SEGMENT_PIN_c] = PIN2,
            [H7SEGMENT_PIN_d] = PIN3,
            [H7SEGMENT_PIN_e] = PIN4,
            [H7SEGMENT_PIN_f] = PIN5,
            [H7SEGMENT_PIN_g] = PIN6,
            [H7SEGMENT_PIN_dot] = PIN7
        },
        .connection = H7SEGMENT_COMMON_CATHODE
    }
};
