#ifndef MEXTI_H
#define MEXTI_H

/* configuration struct (intNUM + on rising_edge vs falling_edge vs ..) */
/* enum for exti nums */
typedef enum 
{
    MEXTI_OK,
    MEXTI_NULL_PTR,
    MEXTI_INV_TRIG_MODE,
    MEXTI_INV_INT_NUM,
} MEXTI_enuErrorStatus_t;

#define MEXTI_INT0 MGIE_EXTI0
#define MEXTI_INT1 MGIE_EXTI1
#define MEXTI_INT2 MGIE_EXTI2

typedef enum 
{
    MEXTI_LOW_LEVEL,
    MEXTI_ON_CHANGE,
    MEXTI_FALLING_EDGE,
    MEXTI_RISING_EDGE,
} MEXTI_enuIntSenseCtrl_t;

typedef struct 
{
    uint8_t IntNum;
    MEXTI_enuIntSenseCtrl_t TriggerMode;
} MEXTI_structIntConfig_t;

extern MEXTI_enuErrorStatus_t MEXTI_enuInit(MEXTI_structIntConfig_t* IntConfig);
extern MEXTI_enuErrorStatus_t MEXTI_enuRegisterCallBackFunc(uint8_t Copy_uint8IntNum, MGIE_CallBackFunction_t Ptr_ExIntCallBackFunc);

#endif