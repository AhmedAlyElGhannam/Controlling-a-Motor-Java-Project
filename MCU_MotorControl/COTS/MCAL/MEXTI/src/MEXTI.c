#include "std_types.h"
#include "MGIE.h"
#include <util/delay.h>
#include "MEXTI_Registers.h"
#include "MEXTI.h"

extern MGIE_CallBackFunction_t MGIE_CallBackFunctions[MGIE_NUM_OF_INTERRUPTS];

MEXTI_enuErrorStatus_t MEXTI_enuInit(MEXTI_structIntConfig_t* Ptr_structIntConfig)
{
    MEXTI_enuErrorStatus_t Local_enuErrorStatus = MEXTI_OK;
    volatile MEXTI_strRegisters_t* EXTI = (volatile MEXTI_strRegisters_t*)MEXTI_BASE_ADDRESS;

    if (NULL == Ptr_structIntConfig)
    {
        Local_enuErrorStatus = MEXTI_NULL_PTR;
    }
    else
    {
        switch (Ptr_structIntConfig->IntNum)
        {
            case MEXTI_INT0:
                EXTI->MCUCR = (EXTI->MCUCR & 0xFC) | Ptr_structIntConfig->TriggerMode;
            break;
            
            case MEXTI_INT1:
                EXTI->MCUCR = (EXTI->MCUCR & 0xF3) | (Ptr_structIntConfig->TriggerMode << 2);
            break;

            case MEXTI_INT2:
                if ((Ptr_structIntConfig->TriggerMode != MEXTI_RISING_EDGE) && (Ptr_structIntConfig->TriggerMode != MEXTI_FALLING_EDGE))
                {
                    Local_enuErrorStatus = MEXTI_INV_TRIG_MODE;
                }
                else 
                {
                    EXTI->MCUCSR = (EXTI->MCUCSR & 0xBF) | ((Ptr_structIntConfig->TriggerMode & 1U) << 6);
                }
            break;

            default:
                Local_enuErrorStatus = MEXTI_INV_INT_NUM;
            break;
        }
    }

    return Local_enuErrorStatus;
}

MEXTI_enuErrorStatus_t MEXTI_enuRegisterCallBackFunc(uint8_t Copy_uint8IntNum, MGIE_CallBackFunction_t Ptr_ExIntCallBackFunc)
{
    MEXTI_enuErrorStatus_t Local_enuErrorStatus = MEXTI_OK;

    if ((Copy_uint8IntNum > MEXTI_INT2) || (Copy_uint8IntNum < MEXTI_INT0))
    {
        Local_enuErrorStatus = MEXTI_INV_INT_NUM;
    }
    else if (Ptr_ExIntCallBackFunc == NULL)
    {
        Local_enuErrorStatus = MEXTI_NULL_PTR;
    }
    else 
    {
        MGIE_CallBackFunctions[Copy_uint8IntNum] = Ptr_ExIntCallBackFunc;
    }

    return Local_enuErrorStatus;
}

void __vector_1(void) __attribute__((signal));
void __vector_1(void)
{
    if (MGIE_CallBackFunctions[MEXTI_INT0] != NULL)
    {
        MGIE_CallBackFunctions[MEXTI_INT0]();
    }
    else {}
}
void __vector_2(void) __attribute__((signal));
void __vector_2(void)
{
    if (MGIE_CallBackFunctions[MEXTI_INT1] != NULL)
    {
        MGIE_CallBackFunctions[MEXTI_INT1]();
    }
    else {}
}
void __vector_3(void) __attribute__((signal));
void __vector_3(void)
{
    if (MGIE_CallBackFunctions[MEXTI_INT2] != NULL)
    {
        MGIE_CallBackFunctions[MEXTI_INT2]();
    }
    else {}
}
