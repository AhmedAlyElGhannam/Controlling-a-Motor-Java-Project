#ifndef TIMER0_CONFIG_H
#define TIMER0_CONFIG_H


// Timer0 Modes
typedef enum {
    TIMER0_NORMAL_MODE,            // Normal mode
    TIMER0_CTC_MODE,               // Clear Timer on Compare Match (CTC) mode
    TIMER0_FAST_PWM_MODE,          // Fast PWM mode
    TIMER0_PHASE_CORRECT_PWM_MODE  // Phase Correct PWM mode
} TIMER0_Mode;

// Timer0 Prescaler Options
typedef enum {
    TIMER0_NO_CLOCK,               // No clock source (Timer/Counter stopped)
    TIMER0_NO_PRESCALER,           // No prescaling (clock = F_CPU)
    TIMER0_PRESCALER_8,            // Prescaler = 8
    TIMER0_PRESCALER_64,           // Prescaler = 64
    TIMER0_PRESCALER_256,          // Prescaler = 256
    TIMER0_PRESCALER_1024,         // Prescaler = 1024
    TIMER0_EXTERNAL_CLOCK_FALLING, // External clock on T0 pin, falling edge
    TIMER0_EXTERNAL_CLOCK_RISING   // External clock on T0 pin, rising edge
} TIMER0_Prescaler;



// Timer0 Configuration Structure
typedef struct {
    TIMER0_Mode mode;       // modes (Normal, CTC, Fast PWM, Phase Correct PWM)
    TIMER0_Prescaler prescaler; // prescaler
    u8 compareValue;        // Compare value for CTC and PWM modes
    u8 dutyCycle;           // Duty cycle for PWM modes
} TIMER0_ConfigType;

#endif // TIMER0_CONFIG_H