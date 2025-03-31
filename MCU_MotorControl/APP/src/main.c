#include "std_types.h"
#include "bit_math.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include "MPORT.h"
#include "MDIO.h"
#include "HLED.h"
#include "MGIE.h"
#include "MEXTI.h"
#include "MTIMER.h"
#include "HSCHEDULER.h"
#include "usart.h"
#include "Motor.h"


// Main Test Program
int main(void) {
	Motor_Init();
	
	while(1) {
		// Forward acceleration
		Motor_SetDirection(Forward);
		for(u8 speed = 0; speed <= 100; speed += 10) {
			Motor_SetSpeed(speed);
			_delay_ms(250);
		}
		
		// Deceleration
		for(u8 speed = 100; speed > 0; speed -= 10) {
			Motor_SetSpeed(speed);
			_delay_ms(250);
		}
		
		_delay_ms(1000);
		// Reverse direction
		// Motor_SetDirection(Backward);
		// Motor_SetSpeed(70); // Constant reverse speed
		Motor_SetSpeed(100);
		_delay_ms(1000);
		
		//  brake test
		Motor_Brake();
		_delay_ms(1000);
	}
}