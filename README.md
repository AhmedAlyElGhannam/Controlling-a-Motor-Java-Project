# Controlling-a-Motor-Java-Project

# Tasks --- Java:
- [ ] Class for USART communication &rarr; [sendByte() && receiveByte()] [set baudrate to 9600 + 1 start + 1 stop + 8 bit-data + 0 parity] **A**
- [x] GUI for Required Scene. **R**
- [ ] Slider handler for required scene. **O**
- [x] TextField for speed + dir. **R**
- [ ] TextField for acknowledgement. **R**
- [ ] EXTRA: GUI for Air Conditioner
- [ ] EXTRA: Handlers for Knob + Toggle Switch

# Tasks --- MCU Motor Control:
- [x] USART Driver.
- [ ] PWM Driver. **R**
- [x] Scheduler (Timer) Driver.
- [x] DIO/GPIO Driver for LEDs.
- [ ] Receiver logic for speed/dir/id extraction/validation. **O**
- [ ] Sender logic for acknowledge frame. **A**
- [ ] PWM signal for motor control (speed/dir). **R**

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
