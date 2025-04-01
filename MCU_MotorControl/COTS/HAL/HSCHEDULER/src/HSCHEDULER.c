#include "std_types.h"
#include "MGIE.h"
#include "MTIMER.h"
#include "HSCHEDULER_LCFG.h"
#include "HSCHEDULER.h"

extern HSCHEDULER_structRunnable_t HSCHEDULER_structRunnableArr[NUM_OF_RUNNABLES];
uint32_t global_uint32OverflowCount = 0;

// HSCHEDULER_enuErrorStatus_t HSCHEDULER_enuRegisterRunnable(HSCHEDULER_structRunnable_t* ptr_structRunnable)
// {

// }

void HSCHEDULER_voidInit(void)
{
    /* disable interrupts */
    cli();

    /* initialize TIMER2 to Normal (OVF) mode */
    MTIMER_enuInit(TIMER2);

    /* set timer callback to HSCHEDULER_voidStartRunnable */
    MTIMER_enuSetTimerCallBack(HSCHEDULER_voidStartRunnable, MGIE_TIMER2_OVERFLOW);

    /* set scheduler time to 1ms */
    MTIMER_enuSetTimeMs(TIMER2, 1, &global_uint32OverflowCount); 

    /* enable interrupts */
    sei();

    /* infinite loop to prevent this function from exiting */
    while(true);
}

void HSCHEDULER_voidStartRunnable(void)
{
    static uint32_t local_uint32PassedMs = 0;
    static uint32_t local_uint32Counter = 0;
    uint8_t local_uint8Iter;

    /* function counter must reach number of overflows */
    if (local_uint32Counter == global_uint32OverflowCount)
    {
        /* iterate over runnable array */
        for (local_uint8Iter = 0; local_uint8Iter < NUM_OF_RUNNABLES; local_uint8Iter++)
        {
            /* first delay must be 0 to proceed */
            if (HSCHEDULER_structRunnableArr[local_uint8Iter].firstDelay == 0)
            {
                /* proceed if passedMs is divisible by runnable period && passedMs is not 0 */
                if ((local_uint32PassedMs % HSCHEDULER_structRunnableArr[local_uint8Iter].period == 0) && (local_uint32PassedMs != 0))
                {
                    /* proceed IFF runnable cbf is not NULL */
                    if (HSCHEDULER_structRunnableArr[local_uint8Iter].cbf != NULL)
                    {
                        /* call runnable cbf */
                        HSCHEDULER_structRunnableArr[local_uint8Iter].cbf();
                    }
                    else {} /* do nothing */
                }
                else {} /* do nothing */
            }
            else 
            {
                /* keep decrementing first delay */
                HSCHEDULER_structRunnableArr[local_uint8Iter].firstDelay--;
            }
        } 

        /* increment passedMs */
        local_uint32PassedMs++;
        
        /* reset counter */
        local_uint32Counter = 0;
    }
    else 
    {
        /* keep incrementing counter until it reaches overflowCount */
        local_uint32Counter++;
    }
}
