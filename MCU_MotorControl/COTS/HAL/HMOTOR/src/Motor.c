#include "std_types.h"
#include "bit_math.h"
#include "MPORT.h"
#include "MDIO.h"
#include "TIMER0.h"
#include "TIMER0_Config.h"
#include "Motor.h"

// Motor Control Functions
void Motor_Init(void) {
	// Initialize PWM on OC0 (PB3) for motor speed control
	MPORT_enuSetPinDirection(MPORT_PIN_B3, MPORT_PORT_PIN_OUTPUT); // PWM pin
	MPORT_enuSetPinDirection(MPORT_PIN_B0, MPORT_PORT_PIN_OUTPUT); // DIR pin 1
	MPORT_enuSetPinDirection(MPORT_PIN_B1, MPORT_PORT_PIN_OUTPUT); // DIR pin 2
	
	// Fast PWM, 8kHz frequency (~64 prescaler @16MHz) (I am operating at 8MHz)
	Timer0_Init_FastPWM(TIMER0_PRESCALER_64, 0);
	Timer0_SetPWMPolarity(PWM_POLARITY_NON_INVERTED);
	Timer0_Start();
}

void Motor_SetSpeed(u8 speed) {
	// Constrain speed to 0-100%
	if(speed > 100) speed = 100;
	Timer0_SetDutyCycle(speed); // Atomic update
}


void Motor_SetDirection(int dir) {
	if(dir) {
		MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN0, MDIO_PIN_HIGH);
		MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN1, MDIO_PIN_LOW);
		} else {
		MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN0, MDIO_PIN_LOW);
		MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN1, MDIO_PIN_HIGH);
	}
}

void Motor_Brake(void) {
	MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN0, MDIO_PIN_HIGH);
	MDIO_enuSetPinValue(MDIO_PORTB, MDIO_PIN1, MDIO_PIN_HIGH);
	Motor_SetSpeed(0);
}