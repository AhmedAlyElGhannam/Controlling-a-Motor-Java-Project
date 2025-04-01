# Controlling-a-Motor-Java-Project

# Tasks --- Java:
- [x] SerialCommManager class for handling USART communication periodically.  **A**
- [x] GUI for Required Scene. **R**
- [x] Slider handler for required scene. **O**
- [x] TextField for speed + dir. **R**
- [x] App integration with SerialCommManager. **A** 
- [ ] TextField for acknowledgement. **R**
- [ ] EXTRA: GUI for Air Conditioner
- [ ] EXTRA: Handlers for Knob + Toggle Switch

# Tasks --- MCU Motor Control:
- [x] USART Driver. **O**
- [x] PWM Driver. **R**
- [x] Scheduler (Timer) Driver. 
- [x] DIO/GPIO Driver for LEDs. 
- [ ] USART IRQ that handles data reception && ack byte transmission. **O**
- [ ] Scheduled function that does the speed control logic. **A**

# JavaFX
1. Class for USART communication ###
1. Class for App (With 2 scenes):
	1. Required. ###
	1. Spice (Air Conditioning)
1. [Thread] Handler for Slider to get value from defined range + send it via USART **SOMEHOW** ###
1. TextField to show motor rpm + CW || CCW. ###
1. Additional TextField to indicate that an acknowledge frame was received from MCU.
1. Handler for Spice page switcher button
	1. Knob,
	1. On/Off toggle switch,
	1. CW for Cold && CCW for Hot.

# MCU [ATMEGA32 --- debatable]
1. Drivers: (USART + PWM + Sched [TIMER] + DIO/GPIO [for LEDs])
1. Every 500ms, read received USART frame and set motor speed accordingly.
1. USART frame consists of the following fields:
	1. Bit 7-5 -> ID &rarr; changes with every sent frame **if receivedID == lastID ==> then connection is lost [LED Red]** *else ==> everything is ok [LED Green]*
	1. Bit 4 -> Dir &rarr; CW || CCW
	1. Bit 3-0 -> Speed &rarr; values from 0 to 15 will be scaled accordingly.
1. PWM signal should use received `speed` && `dir` fields to drive the motor.
1. Upon receiving a valid frame from JavaFX app, send a USART acknowledgement frame `0xFF` to GUI app.

# MCU [Actual Requirements]
1. Scheduler -> every 500ms run function

> USART IRQ:
1. upon receiving a byte (data), save it in some global variable THEN send Ack.

> Scheduled Function:
1. [Alternative --- NOT NEEDED **if USART IRQ is used**] wait until data byte is received THEN send Ack.
1. Extract data from received byte.
1. If ID is the same as old ID &rarr; stop motor, light up a red LED, return [This makes MCU check for received byte each time Scheduled function is invoked to resume immediately after connection is back].
1. Else, Drive motor with specified speed and direction + light up a green LED.
