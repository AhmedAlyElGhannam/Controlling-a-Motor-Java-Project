#include "std_types.h"
#include "bit_math.h"
#include "MPORT.h"
#include "TIMER0.h"
#include "TIMER0_Config.h"
#include "TIMER0_Private.h"


// Global callback function pointers
static Timer0_Callback overflowCallback = NULL;
static Timer0_Callback compareMatchCallback = NULL;

// Set the overflow callback
TIMER0_ERROR_STATUS Timer0_SetOverflowCallback(Timer0_Callback callback) {
	if (callback == NULL) {
		return TIMER0_CALLBACK_NOT_SET;
	}
	overflowCallback = callback;
	return TIMER0_OK;
}

// Set the compare match callback
TIMER0_ERROR_STATUS Timer0_SetCompareMatchCallback(Timer0_Callback callback) {
	if (callback == NULL) {
		return TIMER0_CALLBACK_NOT_SET;
	}
	compareMatchCallback = callback;
	return TIMER0_OK;
}

// Timer0 overflow interrupt service routine
void __vector_11(void) __attribute__((signal, used));
void __vector_11(void) {
	if (overflowCallback != NULL) {
		overflowCallback(); // Call the overflow callback
	}
}

// Timer0 compare match interrupt service routine
void __vector_10(void) __attribute__((signal, used));
void __vector_10(void) {
	if (compareMatchCallback != NULL) {
		compareMatchCallback(); // Call the compare match callback
	}
}

// Enable global interrupts
void Timer0_EnableGlobalInterrupts(void) {
	__asm__ __volatile__ ("sei" ::: "memory");
}

// Disable global interrupts
void Timer0_DisableGlobalInterrupts(void) {
	__asm__ __volatile__ ("cli" ::: "memory");
}

// Initialize Timer0 in Normal Mode
TIMER0_ERROR_STATUS Timer0_Init_NormalMode(u8 prescaler) {
	if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) {
		return TIMER0_INVALID_PRESCALER;
	}
	// Clear mode and prescaler bits, then set prescaler
	TCCR0 &= ~((1 << WGM01) | (1 << WGM00)); // Clear mode bits
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Clear prescaler bits
	TCCR0 |= prescaler; // Set prescaler
	return TIMER0_OK;
}

// Initialize Timer0 in CTC Mode
TIMER0_ERROR_STATUS Timer0_Init_CTCMode(u8 prescaler, u8 compareValue) {
	if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) {
		return TIMER0_INVALID_PRESCALER;
	}
	if (compareValue > 255) {
		return TIMER0_INVALID_COMPARE_VALUE;
	}
	// Clear mode and prescaler bits, then set CTC mode and prescaler
	TCCR0 &= ~((1 << WGM01) | (1 << WGM00)); // Clear mode bits
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Clear prescaler bits
	TCCR0 |= (1 << WGM01) | prescaler; // Set CTC mode and prescaler
	OCR0 = compareValue; // Set compare value
	return TIMER0_OK;
}

// Initialize Timer0 in Fast PWM Mode
TIMER0_ERROR_STATUS Timer0_Init_FastPWM(u8 prescaler, u8 dutyCycle) {
	if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) {
		return TIMER0_INVALID_PRESCALER;
	}
	if (dutyCycle > 100) {
		return TIMER0_INVALID_DUTY_CYCLE;
	}
	// Clear mode and prescaler bits, then set Fast PWM mode and prescaler
	TCCR0 &= ~((1 << WGM01) | (1 << WGM00)); // Clear mode bits
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Clear prescaler bits
	TCCR0 |= (1 << WGM01) | (1 << WGM00) | prescaler; // Set Fast PWM mode and prescaler
	OCR0 = (dutyCycle * 255) / 100; // Set duty cycle
	return TIMER0_OK;
}

// Initialize Timer0 in Phase Correct PWM Mode
TIMER0_ERROR_STATUS Timer0_Init_PhaseCorrectPWM(u8 prescaler, u8 dutyCycle) {
	if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) {
		return TIMER0_INVALID_PRESCALER;
	}
	if (dutyCycle > 100) {
		return TIMER0_INVALID_DUTY_CYCLE;
	}
	// Clear mode and prescaler bits, then set Phase Correct PWM mode and prescaler
	TCCR0 &= ~((1 << WGM01) | (1 << WGM00)); // Clear mode bits
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Clear prescaler bits
	TCCR0 |= (1 << WGM00) | prescaler; // Set Phase Correct PWM mode and prescaler
	OCR0 = (dutyCycle * 255) / 100; // Set duty cycle
	return TIMER0_OK;
}

// Start Timer0
TIMER0_ERROR_STATUS Timer0_Start(void) {
	if (TCCR0 & ((1 << CS02) | (1 << CS01) | (1 << CS00))) {
		return TIMER0_BUSY;
	}
	TCCR0 |= (1 << CS00); // Start Timer0 with the configured prescaler
	return TIMER0_OK;
}

// Stop Timer0
TIMER0_ERROR_STATUS Timer0_Stop(void) {
	if (!(TCCR0 & ((1 << CS02) | (1 << CS01) | (1 << CS00)))) {
		return TIMER0_BUSY;
	}
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Stop Timer0
	return TIMER0_OK;
}

// Set Timer0 prescaler
TIMER0_ERROR_STATUS Timer0_SetPrescaler(u8 prescaler) {
	if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) {
		return TIMER0_INVALID_PRESCALER;
	}
	TCCR0 &= ~((1 << CS02) | (1 << CS01) | (1 << CS00)); // Clear prescaler bits
	TCCR0 |= prescaler; // Set new prescaler
	return TIMER0_OK;
}

// Set Timer0 compare value
TIMER0_ERROR_STATUS Timer0_SetCompareValue(u8 compareValue) {
	if (compareValue > 255) {
		return TIMER0_INVALID_COMPARE_VALUE;
	}
	OCR0 = compareValue; // Set compare value
	return TIMER0_OK;
}

// Set Timer0 duty cycle
TIMER0_ERROR_STATUS Timer0_SetDutyCycle(u8 dutyCycle) {
	if (dutyCycle > 100) {
		return TIMER0_INVALID_DUTY_CYCLE;
	}
	OCR0 = (dutyCycle * 255) / 100; // Set duty cycle
	return TIMER0_OK;
}


TIMER0_ERROR_STATUS Timer0_SetPWMPolarity(PWM_Polarity polarity) {
    // Clear COM bits first
    TCCR0 &= ~((1 << COM01) | (1 << COM00));
    
    if (polarity == PWM_POLARITY_NON_INVERTED) {
        TCCR0 |= (1 << COM01);  // Non-inverted PWM
    } else {
        TCCR0 |= (1 << COM01) | (1 << COM00);  // Inverted PWM
    }
    return TIMER0_OK;
}

// Call this during PWM initialization
TIMER0_ERROR_STATUS Timer0_Init_PWM(u8 prescaler, u8 dutyCycle, PWM_Polarity polarity) {
    // Set OC0 (PB3) as output - REQUIRED for PWM to work!    
	MPORT_enuSetPinDirection(MPORT_PIN_B3, MPORT_PORT_PIN_OUTPUT);

    // Rest of your existing PWM init code
    if (prescaler > TIMER0_EXTERNAL_CLOCK_RISING) return TIMER0_INVALID_PRESCALER;
    if (dutyCycle > 100) return TIMER0_INVALID_DUTY_CYCLE;
    
    TCCR0 &= ~((1 << WGM01) | (1 << WGM00));
    TCCR0 |= (1 << WGM01) | (1 << WGM00) | prescaler;
    OCR0 = (dutyCycle * 255) / 100;
    
    // Set polarity
    Timer0_SetPWMPolarity(polarity);
    
    return TIMER0_OK;
}


TIMER0_ERROR_STATUS Timer0_SetPWMFrequency(u32 desiredFreqHz) {
    u32 cpuFreq = F_CPU;  // Ensure F_CPU is defined
    
    // Calculate required prescaler
    u32 prescalers[] = {1, 8, 64, 256, 1024};
    u8 bestPrescaler = 0;
    u32 bestTop = 0;
    
    for (u8 i = 0; i < 5; i++) {
        u32 top = (cpuFreq / (prescalers[i] * desiredFreqHz)) - 1;
        if (top <= 255) {
            bestPrescaler = i;
            bestTop = top;
            break;
        }
    }
    
    if (bestTop == 0) return TIMER0_INVALID_FREQUENCY;
    
    // Apply settings
    Timer0_SetPrescaler(bestPrescaler);
    return TIMER0_OK;
}



// Enable Timer0 overflow interrupt
TIMER0_ERROR_STATUS Timer0_EnableOverflowInterrupt(void) {
	TIMSK |= (1 << TOIE0); // Enable overflow interrupt
	Timer0_EnableGlobalInterrupts(); // Enable global interrupts
	return TIMER0_OK;
}

// Disable Timer0 overflow interrupt
TIMER0_ERROR_STATUS Timer0_DisableOverflowInterrupt(void) {
	TIMSK &= ~(1 << TOIE0); // Disable overflow interrupt
	return TIMER0_OK;
}

// Enable Timer0 compare match interrupt
TIMER0_ERROR_STATUS Timer0_EnableCompareMatchInterrupt(void) {
	TIMSK |= (1 << OCIE0); // Enable compare match interrupt
	Timer0_EnableGlobalInterrupts(); // Enable global interrupts
	return TIMER0_OK;
}

// Disable Timer0 compare match interrupt
TIMER0_ERROR_STATUS Timer0_DisableCompareMatchInterrupt(void) {
	TIMSK &= ~(1 << OCIE0); // Disable compare match interrupt
	return TIMER0_OK;
}

// Get Timer0 counter value
u8 Timer0_GetCounterValue(void) {
	return TCNT0; // Return current counter value
}

// Clear Timer0 counter
TIMER0_ERROR_STATUS Timer0_ClearCounter(void) {
	TCNT0 = 0; // Clear counter
	return TIMER0_OK;
}