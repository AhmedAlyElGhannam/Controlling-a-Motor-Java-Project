#ifndef	  MOTOR_H
#define   MOTOR_H

#define Forward  1
#define Backward 0

void Motor_Init(void);
void Motor_SetSpeed(u8 speed);
void Motor_SetDirection(int dir);
void Motor_Brake(void);


#endif