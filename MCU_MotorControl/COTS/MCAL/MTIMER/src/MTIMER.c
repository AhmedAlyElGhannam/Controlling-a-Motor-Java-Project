#include "bit_math.h"
#include "std_types.h"
#include "MTIMER_Registers.h"
#include "MGIE.h"
#include "MTIMER.h"


extern MGIE_CallBackFunction_t MGIE_CallBackFunctions[MGIE_NUM_OF_INTERRUPTS];
extern MTIMER_structTimerConfig_t global_structTimers[NUM_OF_TIMERS];

static void MTIMER_voidTimer0Init(void)
{
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;
    volatile uint8_t local_uint8RegTemp = 0x00;

    /* timer mode */
    switch (global_structTimers[TIMER0].timerMode)
    {
        case TIMER_OVF_MODE:
            /* no need to do a thing */
            CLR_BIT(local_uint8RegTemp, 3);
            CLR_BIT(local_uint8RegTemp, 6);
        break;

        case TIMER_PWM_PHASE_CORRECT_MODE:
            /* set bit 3 */
            SET_BIT(local_uint8RegTemp, 3);
            CLR_BIT(local_uint8RegTemp, 6);
        break;

        case TIMER_CTC_MODE:
            /* set bit 6 */
            CLR_BIT(local_uint8RegTemp, 3);
            SET_BIT(local_uint8RegTemp, 6);
        break;

        case TIMER_FAST_PWM_MODE:
            /* set bit 3 & 6 */
            SET_BIT(local_uint8RegTemp, 3);
            SET_BIT(local_uint8RegTemp, 6);
        break;
    }

    /* clock selection */
    local_uint8RegTemp |= global_structTimers[TIMER0].timerClkSel;

    /* set temp value into register */
    MTIMER->TCCR0 = local_uint8RegTemp;

    /* configure interrupt based on mode of operation */
    /* this is purely for testing purposes */
    SET_BIT(MTIMER->TIMSK, 0);
    // MTIMER->TIMSK = (MTIMER->TIMSK & (~11U)) |  (global_structTimers[TIMER0].timerIntState << global_structTimers[TIMER0].timerMode);
}

static void MTIMER_voidTimer1Init(void)
{
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;
    volatile uint8_t local_uint8RegTemp = 0x00;

    /* timer mode */
    switch (global_structTimers[TIMER1].timerMode)
    {
        case TIMER_OVF_MODE:
            
        break;

        case TIMER_PWM_PHASE_CORRECT_MODE:
            
        break;

        case TIMER_CTC_MODE:
           
        break;

        case TIMER_FAST_PWM_MODE:
            
        break;

        case TIMER_ICU_MODE:
            SET_BIT(MTIMER->TCCR1B, CAPTURE_RISING_EDGE);
        break;
    }
}

static void MTIMER_voidTimer2Init(void)
{
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;
    volatile uint8_t local_uint8TCCRTemp = 0x00;

    /* timer mode */
    switch (global_structTimers[TIMER2].timerMode)
    {
        case TIMER_OVF_MODE:
            /* no need to do a thing */
            CLR_BIT(local_uint8TCCRTemp, 3);
            CLR_BIT(local_uint8TCCRTemp, 6);
        break;

        case TIMER_PWM_PHASE_CORRECT_MODE:
            /* set bit 3 and clear bit 6 to set mode */
            SET_BIT(local_uint8TCCRTemp, 3);
            CLR_BIT(local_uint8TCCRTemp, 6);
            /* set compare out mode to non-inverting PWM */
            SET_BIT(local_uint8TCCRTemp, 5);
            CLR_BIT(local_uint8TCCRTemp, 4);
        break;

        case TIMER_CTC_MODE:
            /* set bit 6 */
            CLR_BIT(local_uint8TCCRTemp, 3);
            SET_BIT(local_uint8TCCRTemp, 6);
        break;

        case TIMER_FAST_PWM_MODE:
            /* set bit 3 & 6 */
            SET_BIT(local_uint8TCCRTemp, 3);
            SET_BIT(local_uint8TCCRTemp, 6);
        break;
    }

    /* clock selection */
    local_uint8TCCRTemp |= global_structTimers[TIMER2].timerClkSel;

    /* set temp value into register */
    MTIMER->TCCR2 = local_uint8TCCRTemp;

    /* enable source of interrupt */
    SET_BIT(MTIMER->TIMSK, 6);
    // SET_BIT(MTIMER->TIMSK, 7);
    /* configure interrupt based on mode of operation (NEEDS TO BE EDITED TO MAKE SURE IT IS OK) */
    // MTIMER->TIMSK = (MTIMER->TIMSK & (~11U)) |  (global_structTimers[TIMER2].timerIntState << global_structTimers[TIMER2].timerMode);
}

MTIMER_enuErrorStatus_t MTIMER_enuInit(MTIMER_enuTimers_t copy_enuTimer)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;

    switch (copy_enuTimer)
    {
        case TIMER0:
            MTIMER_voidTimer0Init();
        break;

        case TIMER1:
            MTIMER_voidTimer1Init();
        break;

        case TIMER2:
            MTIMER_voidTimer2Init();
        break;

        default:
            ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }

    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuSetOverflowVal(MTIMER_enuTimers_t copy_enuTimer, uint32_t copy_uint32PreloadVal)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;

    switch (copy_enuTimer)
    {
        case TIMER0:
            MTIMER->TCNT0 = (uint8_t)(0xFF & copy_uint32PreloadVal);
        break;

        case TIMER1:
            MTIMER->TCNT1H = (uint8_t)((0xFF00 & copy_uint32PreloadVal) >> 8);
            MTIMER->TCNT1L = (uint8_t)(0xFF & copy_uint32PreloadVal);
        break;

        case TIMER2:
            MTIMER->TCNT2 = (uint8_t)(0xFF & copy_uint32PreloadVal);
        break;

        default:
            ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }

    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuSetClkSelection(MTIMER_enuTimers_t copy_enuTimer, MTIMER_enuClockSelection_t copy_enuClkSel)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;

    switch (copy_enuTimer)
    {
        case TIMER0:
            MTIMER->TCCR0 |= global_structTimers[TIMER0].timerClkSel;
        break;

        case TIMER1:
            MTIMER->TCCR1B |= global_structTimers[TIMER1].timerClkSel;
        break;

        case TIMER2:
            MTIMER->TCCR2 |= global_structTimers[TIMER2].timerClkSel;
        break;

        default:
            ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }

    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuSetOutputCompareVal(MTIMER_enuTimers_t copy_enuTimer, uint8_t copy_uint32PreloadVal)
{
	MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;

    switch (copy_enuTimer)
    {
        case TIMER0:
            MTIMER->OCR0 = (uint8_t)(0xFF & copy_uint32PreloadVal);
        break;

        case TIMER1:
            /* will be left on hold for now */
            /*MTIMER->OCR1H = (uint8_t)((0xFF00 & copy_uint32PreloadVal) >> 8);
            MTIMER->OCR1L = (uint8_t)(0xFF & copy_uint32PreloadVal);*/
        break;

        case TIMER2:
            MTIMER->OCR2 = (uint8_t)(0xFF & copy_uint32PreloadVal);
        break;

        default:
            ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }

    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuSetPWMDutyCycle(MTIMER_enuTimers_t copy_enuTimer, MTIMER_enuPWMDutyCycle_t copy_enuDutyCycle)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;
    uint32_t local_uint32DutyCycleCmpVal = 0;

    if (copy_enuDutyCycle > PWM_DUTY_CYCLE_100)
    {
        ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }
    else 
    {
        switch (copy_enuTimer)
        {
            case TIMER0:
                local_uint32DutyCycleCmpVal = (255UL * copy_enuDutyCycle) / 100U;
                MTIMER->OCR0 = (uint8_t)(local_uint32DutyCycleCmpVal & 0xFF);
            break;

            case TIMER1:
                local_uint32DutyCycleCmpVal = (65536UL * copy_enuDutyCycle) / 100;
                // MTIMER->OCR1
            break;

            case TIMER2:
                local_uint32DutyCycleCmpVal = (255UL * copy_enuDutyCycle) / 100;
                MTIMER->OCR2 = (uint8_t)(local_uint32DutyCycleCmpVal & 0xFF);
            break;

            default:
                ret_enuErrorStatus = MTIMER_INVALID_PARAM;
        }
    }

    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuGetCounterVal(MTIMER_enuTimers_t copy_enuTimer, uint32_t* ptr_uint32CounterVal)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;

    if (ptr_uint32CounterVal == NULL)
    {
        ret_enuErrorStatus = MTIMER_NULL_PTR;
    }
    else 
    {
        switch (copy_enuTimer)
        {
            case TIMER0:
                (*ptr_uint32CounterVal) = MTIMER->TCNT0;
            break;

            case TIMER1:
                /* will be left on hold for now */
                /*MTIMER->OCR1H = (uint8_t)((0xFF00 & copy_uint32PreloadVal) >> 8);
                MTIMER->OCR1L = (uint8_t)(0xFF & copy_uint32PreloadVal);*/
                (*ptr_uint32CounterVal) = (MTIMER->TCNT1L | (MTIMER->TCNT1H << 8));
            break;

            case TIMER2:
            (*ptr_uint32CounterVal) = MTIMER->TCNT2;
            break;

            default:
                ret_enuErrorStatus = MTIMER_INVALID_PARAM;
        }
    }
    
    return ret_enuErrorStatus;
}


static uint32_t ceil(float32_t num) 
{
    /* get the integer part of the number */
    int int_part = (int)num; 
    
    /* if the number is positive and has a decimal part, round up */
    if (num > int_part) 
    {
        int_part += 1;
    }
    else {}

    return int_part;
}

static uint8_t getTimerPrescaler(MTIMER_enuTimers_t copy_enuTimer)
{
    if ((copy_enuTimer == TIMER0) || (copy_enuTimer == TIMER2))
    {
        return 8;
    }
}

/* time is in ms */
MTIMER_enuErrorStatus_t MTIMER_enuSetTimeMs(MTIMER_enuTimers_t copy_enuTimer, uint32_t copy_uint32Time, uint32_t* ptr_uint32OverflowCount)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;
    volatile MTIMER_strRegisters_t* MTIMER = (volatile MTIMER_strRegisters_t*)MTIMER_BASE_ADDRESS;

    if ((copy_enuTimer != TIMER0) && (copy_enuTimer != TIMER1) && (copy_enuTimer != TIMER2))
    {
        ret_enuErrorStatus = MTIMER_INVALID_PARAM;
    }
    else if (ptr_uint32OverflowCount == NULL)
    {
        ret_enuErrorStatus = MTIMER_NULL_PTR;
    }
    else 
    {
        /* tick time (1us) */
        uint32_t local_uint32TickTime = (1000000UL) / (F_CPU / getTimerPrescaler(copy_enuTimer)); /* result is in us */
        
        /* overflow time */
        uint32_t local_uint32OverflowTime = 0;
        if ((copy_enuTimer == TIMER0) || (copy_enuTimer == TIMER2))
        {
            /* timer 0  && 2 resolution is 2^8 */
            local_uint32OverflowTime = local_uint32TickTime * 256; /* 256 * 1us */
        }
        else if (copy_enuTimer == TIMER1)
        {
            /* timer 1 resolution is 2^16 */
            local_uint32OverflowTime = local_uint32TickTime * 65536UL; /* 65536 * 1us */
        }
        else {}

        /* calculate num of overflows (all units must be in us) */
        (*ptr_uint32OverflowCount) = ceil((float32_t)((copy_uint32Time * 1000UL) / local_uint32OverflowTime)); /* (time_ms * 1000) / 1 */

        /* calculate fraction of num of OVFs*/
        float32_t local_float32NumOfOVFsFrac = ((float32_t)((copy_uint32Time * 1000UL) / local_uint32OverflowTime)) - (*ptr_uint32OverflowCount);

        /* calculate preload val */
        uint32_t local_uint32PreloadVal = 0;
        if ((copy_enuTimer == TIMER0) || (copy_enuTimer == TIMER2))
        {
            /* timer 0  && 2 resolution is 2^8 */
            local_uint32PreloadVal = 256 - (local_float32NumOfOVFsFrac * 256); 
        }
        else if (copy_enuTimer == TIMER1)
        {
            /* timer 1 resolution is 2^16 */
            local_uint32PreloadVal = 65536UL - (local_float32NumOfOVFsFrac * 65536UL); 
        }
        else {}

        /* set overflow val */
        MTIMER_enuSetOverflowVal(copy_enuTimer, local_uint32PreloadVal);
    }
    
    return ret_enuErrorStatus;
}

MTIMER_enuErrorStatus_t MTIMER_enuSetTimerCallBack(MGIE_CallBackFunction_t ptr_ISR, MGIE_enuVectorTable_t copy_enuISRNum)
{
    MTIMER_enuErrorStatus_t ret_enuErrorStatus = MTIMER_OK;

    if (ptr_ISR == NULL)
    {
        ret_enuErrorStatus = MTIMER_NULL_PTR;
    }
	else
    {
		MGIE_CallBackFunctions[copy_enuISRNum] = ptr_ISR;
    }

    return ret_enuErrorStatus;
}

void __vector_11 (void)	__attribute__((signal));
void __vector_11 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER0_OVERFLOW] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER0_OVERFLOW]();
    }
    else {}
}

void __vector_10 (void)	__attribute__((signal));
void __vector_10 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER0_COMPARE_MATCH] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER0_COMPARE_MATCH]();
    }
    else {}
}

void __vector_9 (void) __attribute__((signal));
void __vector_9 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER1_OVERFLOW] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER1_OVERFLOW]();
    }
    else {}
}

void __vector_8 (void) __attribute__((signal));
void __vector_8 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER1_COMPARE_MATCH_B] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER1_COMPARE_MATCH_B]();
    }
    else {}
}

void __vector_7 (void) __attribute__((signal));
void __vector_7 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER1_COMPARE_MATCH_A] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER1_COMPARE_MATCH_A]();
    }
    else {}
}

void __vector_6 (void) __attribute__((signal));
void __vector_6 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER1_CAPTURE_EVENT] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER1_CAPTURE_EVENT]();
    }
    else {}
}

void __vector_5 (void) __attribute__((signal)) ;
void __vector_5 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER2_OVERFLOW] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER2_OVERFLOW]();
    }
    else {}
}

/*TIMER2 CTC Mode ISR*/
void __vector_4 (void) __attribute__((signal));
void __vector_4 (void)
{
	if (MGIE_CallBackFunctions[MGIE_TIMER2_COMPARE_MATCH] != NULL)
    {
        MGIE_CallBackFunctions[MGIE_TIMER2_COMPARE_MATCH]();
    }
    else {}
}
