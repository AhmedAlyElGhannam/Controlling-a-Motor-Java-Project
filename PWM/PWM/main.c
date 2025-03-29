#include "stdtypes.h"
#include "bit_math.h"
#include "TIMER0.h"
#include "TIMER0_Config.h"
#include "DIO.h"
#include "Motor.h"
#include <util/delay.h>




// Main Test Program
int main(void) {
	Motor_Init();
	
	while(1) {
		// Forward acceleration
		Motor_SetDirection(Forward);
		for(u8 speed = 0; speed <= 100; speed += 10) {
			Motor_SetSpeed(speed);
			_delay_ms(10);
		}
		
		// Deceleration
		for(u8 speed = 100; speed > 0; speed -= 10) {
			Motor_SetSpeed(speed);
			_delay_ms(10);
		}
		
		// Reverse direction
		Motor_SetDirection(Backward);
		Motor_SetSpeed(70); // Constant reverse speed
		_delay_ms(100);
		
		//  brake test
		Motor_Brake();
		_delay_ms(100);
	}
}