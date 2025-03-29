#ifndef TIMER0_H
#define TIMER0_H

#define F_CPU 16000000UL

// SREG address for ATmega328P/ATmega32
#define SREG (*(volatile uint8_t*)0x5F)

// Function pointer type for callbacks
typedef void (*Timer0_Callback)(void);

// Timer0 Error Status Enumeration
typedef enum {
	TIMER0_OK,
	TIMER0_NOK,
	TIMER0_BUSY,
	TIMER0_INVALID_PRESCALER,
	TIMER0_INVALID_MODE,
	TIMER0_INVALID_DUTY_CYCLE,
	TIMER0_INVALID_COMPARE_VALUE,
	TIMER0_INTERRUPT_NOT_SUPPORTED,
	TIMER0_CALLBACK_NOT_SET,
	TIMER0_HARDWARE_ERROR,
	TIMER0_UNINITIALIZED,
	TIMER0_INVALID_OPERATION,
	TIMER0_INVALID_FREQUENCY
} TIMER0_ERROR_STATUS;

typedef enum {
	PWM_POLARITY_NON_INVERTED,
	PWM_POLARITY_INVERTED
} PWM_Polarity;

// Function Prototypes
void Timer0_EnableGlobalInterrupts(void);
void Timer0_DisableGlobalInterrupts(void);

// Initialization Functions
TIMER0_ERROR_STATUS Timer0_Init_NormalMode(u8 prescaler);
TIMER0_ERROR_STATUS Timer0_Init_CTCMode(u8 prescaler, u8 compareValue);
TIMER0_ERROR_STATUS Timer0_Init_FastPWM(u8 prescaler, u8 dutyCycle);
TIMER0_ERROR_STATUS Timer0_Init_PhaseCorrectPWM(u8 prescaler, u8 dutyCycle);

// Control Functions
TIMER0_ERROR_STATUS Timer0_Start(void);
TIMER0_ERROR_STATUS Timer0_Stop(void);
TIMER0_ERROR_STATUS Timer0_SetPrescaler(u8 prescaler);
TIMER0_ERROR_STATUS Timer0_SetCompareValue(u8 compareValue);
TIMER0_ERROR_STATUS Timer0_SetDutyCycle(u8 dutyCycle);
TIMER0_ERROR_STATUS Timer0_SetDutyCycle_Safe(u8 dutyCycle);

// PWM Specific Functions
TIMER0_ERROR_STATUS Timer0_SetPWMPolarity(PWM_Polarity polarity);
TIMER0_ERROR_STATUS Timer0_Init_PWM(u8 prescaler, u8 dutyCycle, PWM_Polarity polarity);
TIMER0_ERROR_STATUS Timer0_SetPWMFrequency(u32 desiredFreqHz);
//TIMER0_ERROR_STATUS Timer0_SetDutyCycle_Safe(u8 dutyCycle);

// Interrupt Functions
TIMER0_ERROR_STATUS Timer0_EnableOverflowInterrupt(void);
TIMER0_ERROR_STATUS Timer0_DisableOverflowInterrupt(void);
TIMER0_ERROR_STATUS Timer0_EnableCompareMatchInterrupt(void);
TIMER0_ERROR_STATUS Timer0_DisableCompareMatchInterrupt(void);
TIMER0_ERROR_STATUS Timer0_SetOverflowCallback(Timer0_Callback callback);
TIMER0_ERROR_STATUS Timer0_SetCompareMatchCallback(Timer0_Callback callback);

// Utility Functions
u8 Timer0_GetCounterValue(void);
TIMER0_ERROR_STATUS Timer0_ClearCounter(void);

#endif // TIMER0_H