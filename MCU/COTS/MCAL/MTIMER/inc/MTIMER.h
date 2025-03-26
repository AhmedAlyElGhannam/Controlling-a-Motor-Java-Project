#ifndef MTIMER_H
#define MTIMER_H

#include "MGIE.h"

typedef enum 
{
    TIMER0,
    TIMER1,
    TIMER2,
    NUM_OF_TIMERS,
} MTIMER_enuTimers_t;

typedef enum
{
	MTIMER_OK = 0,
	MTIMER_INVALID_PARAM,
	MTIMER_INVALID_PIN,
	MTIMER_INVALID_PORT,
	MTIMER_NULL_PTR,
	MTIMER_NOK,
} MTIMER_enuErrorStatus_t;

typedef enum 
{
    TIMER_CTC_MODE = 0U,
    TIMER_OVF_MODE,
    TIMER_PWM_PHASE_CORRECT_MODE,
    TIMER_FAST_PWM_MODE,
} MTIMER_enuModeOfOperation_t;

typedef enum 
{
    TIMER_CLK_SEL_NO_CLK = 0U,
    TIMER_CLK_SEL_NO_PRES,
    TIMER_CLK_SEL_DIV_BY_8,
    TIMER_CLK_SEL_DIV_BY_64,
    TIMER_CLK_SEL_DIV_BY_256,
    TIMER_CLK_SEL_DIV_BY_1024,
    TIMER_CLK_SEL_EXT_CLK_FALLING_EDGE,
    TIMER_CLK_SEL_EXT_CLK_RISING_EDGE = 7U,
} MTIMER_enuClockSelection_t;

typedef enum 
{
    TIMER_INT_ENABLE = 1U,
    TIMER_INT_DISABLE = !TIMER_INT_ENABLE,
} MTIMER_enuInterruptOptions_t;

typedef struct 
{
    MTIMER_enuModeOfOperation_t timerMode;
    MTIMER_enuClockSelection_t timerClkSel;
    MTIMER_enuInterruptOptions_t timerIntState;
} MTIMER_structTimerConfig_t;

MTIMER_enuErrorStatus_t MTIMER_enuInit(MTIMER_enuTimers_t copy_enuTimer);
MTIMER_enuErrorStatus_t MTIMER_enuSetOverflowVal(MTIMER_enuTimers_t copy_enuTimer, uint32_t copy_uint32PreloadVal);
MTIMER_enuErrorStatus_t MTIMER_enuSetClkSelection(MTIMER_enuTimers_t copy_enuTimer, MTIMER_enuClockSelection_t copy_enuClkSel);
MTIMER_enuErrorStatus_t MTIMER_enuSetOutputCompareVal(MTIMER_enuTimers_t copy_enuTimer, uint8_t copy_uint32PreloadVal);
MTIMER_enuErrorStatus_t MTIMER_enuGetCounterVal(MTIMER_enuTimers_t copy_enuTimer, uint32_t* ptr_uint32CounterVal);
MTIMER_enuErrorStatus_t MTIMER_enuSetTimerCallBack(MGIE_CallBackFunction_t ptr_ISR, MGIE_enuVectorTable_t copy_enuISRNum);
MTIMER_enuErrorStatus_t MTIMER_enuSetTimeMs(MTIMER_enuTimers_t copy_enuTimer, uint32_t copy_uint32Time, uint32_t* ptr_uint32OverflowCount);

#endif