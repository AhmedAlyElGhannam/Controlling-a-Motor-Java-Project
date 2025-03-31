#include "std_types.h"
#include "HSCHEDULER.h"
#include "HSCHEDULER_LCFG.h"

extern void APP_voidToggleLED(void);

HSCHEDULER_structRunnable_t HSCHEDULER_structRunnableArr[NUM_OF_RUNNABLES] = 
{
    [RUNNABLE_LED] = 
    {
        .period = 1000U,
        .firstDelay = 0,
        .cbf = APP_voidToggleLED
    }
};
