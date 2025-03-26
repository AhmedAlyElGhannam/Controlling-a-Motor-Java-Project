#include "std_types.h"
#include "bit_math.h"
#include <util/delay.h>
#include "MPORT.h"
#include "MDIO.h"
#include "HLED.h"
#include "HLCD.h"
#include "HKEYPAD.h"
#include "MGIE.h"
#include "MEXTI.h"
#include "MTIMER.h"
#include "HSCHEDULER.h"

void timer0_callback(void);

int main(void)
{
	/* disable interrupts */
	cli();

	/* testing Keypad functions */
	HLCD_structLCDObject_t local_structLCDObject = 
	{
		.HLCD_DataPort = PORTA,
		.HLCD_ConfigPort = PORTB,
		.HLCD_RS = PIN0,
		.HLCD_RW = PIN1,
		.HLCD_E  = PIN2
	};

	HKEYPAD_structKeypadObject_t local_structKeypadObject = 
	{
		.RowPinsPort = PORTC,
		.ColPinsPort = PORTC,
		.RowPinsArr = {PIN0, PIN1, PIN2, PIN3},
		.ColPinsArr = {PIN4, PIN5, PIN6, PIN7}
	};

	MEXTI_structIntConfig_t local_structExInt0Config = 
	{
		.IntNum = MEXTI_INT0,
		.TriggerMode = MEXTI_RISING_EDGE
	};

	uint8_t local_uint8PressedKey = '\0';

	/* initialize all pins in mcu */
	MPORT_voidInit();

	/* LED initialization */
	// HLED_voidInit(); /* port D pin 0 */

	/* calling init function for LCD */
	HLCD_enuInit(&local_structLCDObject);

	/* calling init function for keypad */
	HKEYPAD_enuInit(&local_structKeypadObject);

	/* printing a string on first line */
	HLCD_enuWriteString(&local_structLCDObject, "Kotuwaru!");

	/* shifting to line 2 */
	HLCD_enuGoToXY(&local_structLCDObject, HLCD_COL_00, HLCD_LINE_02);

	/* printing an int 
	HLCD_enuWriteInteger(&local_structLCDObject, 86); */

	/* calling exti init */
	MEXTI_enuInit(&local_structExInt0Config);

	/* timer 0 init */
	MTIMER_enuInit(TIMER0);

	/* set callback function */
	MTIMER_enuSetTimerCallBack(timer0_callback, MGIE_TIMER0_OVERFLOW);

	/* set timer overflow */
	MTIMER_enuSetOverflowVal(TIMER0, 64);

	// HLED_uint8SetLEDValue(HLED_START, HLED_ON);

	/* enable interrupts */
	sei();

	/* keep it stuck in an infinite loop */
	while (true)
	{
		/* getting keypad press and printing the pressed char on LCD */
		HKEYPAD_enuGetPressedKey(&local_structKeypadObject, &local_uint8PressedKey);
		
		switch (local_uint8PressedKey)
		{
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case '0':
				HLCD_enuWriteCharacter(&local_structLCDObject, local_uint8PressedKey);				
			break;
		}

		local_uint8PressedKey = '\0';
	}

	return 0;
}

void APP_voidToggleLED(void)
{
	static uint8_t local_boolFlag = 0;
	local_boolFlag ^= 1;
	MDIO_enuSetPinValue(MDIO_PORTD, MDIO_PIN0, local_boolFlag);
}

void timer0_callback(void)
{
	static uint32_t local_uint32Counter = 0;
	static uint8_t local_boolFlag = 0;
	if (local_uint32Counter == 3907)
	{
		local_boolFlag ^= 1;
		MDIO_enuSetPinValue(MDIO_PORTD, MDIO_PIN0, local_boolFlag);
		// HLED_uint8SetLEDValue(HLED_START, local_boolFlag);
		local_uint32Counter = 0;
		MTIMER_enuSetOverflowVal(TIMER0, 64);
	}
	else 
	{
		local_uint32Counter++;
	}

	return;
}