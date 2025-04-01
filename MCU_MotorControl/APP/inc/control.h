#ifndef CONTROL_H
#define CONTROL_H

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

void APP_voidControlSystemInit(void);

#endif