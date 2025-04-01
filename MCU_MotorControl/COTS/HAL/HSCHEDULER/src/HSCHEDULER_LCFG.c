#include "std_types.h"
#include "HSCHEDULER.h"
#include "HSCHEDULER_LCFG.h"

extern void APP_voidScheduledControlFunc(void);

HSCHEDULER_structRunnable_t HSCHEDULER_structRunnableArr[NUM_OF_RUNNABLES] = 
{
    [RUNNABLE_MOTOR_CON] = 
    {
        .period = 500U,
        .firstDelay = 0,
        .cbf = APP_voidScheduledControlFunc
    }
};
