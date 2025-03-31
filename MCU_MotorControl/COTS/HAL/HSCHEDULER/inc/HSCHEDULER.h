#ifndef HSCHEDULER_H
#define HSCHEDULER_H


typedef enum 
{
    RUNNABLE_LED = 0,
    RUNNABLE_LCD,
    RUNNABLE_PWM,
    RUNNABLE_SWITCH,
    NUM_OF_RUNNABLES
} HSCHEDULER_enuRunnableName_t;

typedef void (*HSCHEDULER_CallBackFunction_t) (void);

typedef struct 
{
    HSCHEDULER_CallBackFunction_t cbf;
    uint32_t period;
    uint32_t firstDelay;
} HSCHEDULER_structRunnable_t;

// HSCHEDULER_enuErrorStatus_t HSCHEDULER_enuRegisterRunnable(HSCHEDULER_structRunnable_t* ptr_structRunnable);
void HSCHEDULER_voidInit(void);
void HSCHEDULER_voidStartRunnable(void);

#endif