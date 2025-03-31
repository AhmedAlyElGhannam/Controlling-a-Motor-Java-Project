#include "bit_math.h"
#include "std_types.h"
#include "MTIMER_Registers.h"
#include "MTIMER.h"

MTIMER_structTimerConfig_t global_structTimers[NUM_OF_TIMERS] = 
{
    /* used for scheduler */
    [TIMER2] = {
        .timerClkSel = TIMER_CLK_SEL_DIV_BY_8,
        .timerMode = TIMER_OVF_MODE,
        .timerIntState = TIMER_INT_ENABLE
    },
    /* used for ICU */
    [TIMER1] = {
        .timerClkSel = TIMER_CLK_SEL_DIV_BY_8,
        .timerMode = TIMER_ICU_MODE,
        .timerIntState = TIMER_INT_ENABLE
    },
    /* used for PWM */
    [TIMER0] = {
        .timerClkSel = TIMER_CLK_SEL_DIV_BY_8,
        .timerMode = TIMER_PWM_PHASE_CORRECT_MODE,
        .timerIntState = TIMER_INT_ENABLE
    }
};

