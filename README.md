# Controlling-a-Motor-Java-Project

## Introduction
This is the final project of the Developing Applications Using Java course taught in Information Technology Institute (ITI)'s 9-Month Professional Training Program -- Embedded Systems Track as a part of the intensive Android Automotive subfield under the supervision of Eng. Ahmed Mazen from Java Education & Technology Services department.

## About The Team
This project was made by Team #4 whose members are:
1. [Ahmed Aly El-Ghannam](https://github.com/AhmedAlyElGhannam).
1. [Omar Mohamed Mostafa](https://github.com/omarmohamedmoustafa).
1. [Rahma Abdelkader](https://github.com/rahmaabdelkader2).

## Brief
The project, as the name suggests, is a JavaFX GUI application built to control a DC motor rotation speed and direction---this is done by making the app communicate with an MCU to do the actual motor control logic. Additionally, as a team, we have decided to pick a use case theme for the application as an air conditioner control app. Functionally, it does the exact same job but aesthetically, it makes all the difference. :)

![](./Documentation/SystemBlockDiagram/SystemBD.png)

## Software Technology Stack
The tools, technologies, and libraries used are as follows:
1. JDK 8 + JFX8 &rarr; GUI.
1. Make &rarr; compilation & packaging of GUI and its dependencies into a single `.jar` file. 
1. C11 + CMake + MISRAC Standard &rarr; development of MCU logic and firmware libraries.
1. Python3 &rarr; scripts to test the functionality of each part of the project separately.

## System Architecture
> System architecture can be summarized in the following sequence diagram. For more info, you can view the [project's full documentation](https://github.com/AhmedAlyElGhannam/Controlling-a-Motor-Java-Project/blob/main/Documentation/JavaProjectDoc.pdf).
> 
![](./Documentation/SequenceDiagrams/CombinedSequenceDiagram/FullSD.png)

## Requirements
This section mentions our interpretation of the project's requirements and the design we have followed.
### JavaFX
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

### MCU [ATMEGA32 --- debatable]
1. Drivers: (USART + PWM + Sched [TIMER] + DIO/GPIO [for LEDs])
1. Every 500ms, read received USART frame and set motor speed accordingly.
1. USART frame consists of the following fields:
	1. Bit 7-5 -> ID &rarr; changes with every sent frame **if receivedID == lastID ==> then connection is lost [LED Red]** *else ==> everything is ok [LED Green]*
	1. Bit 4 -> Dir &rarr; CW || CCW
	1. Bit 3-0 -> Speed &rarr; values from 0 to 15 will be scaled accordingly.
1. PWM signal should use received `speed` && `dir` fields to drive the motor.
1. Upon receiving a valid frame from JavaFX app, send a USART acknowledgement frame `0xFF` to GUI app.

### MCU [Actual Requirements]
1. Scheduler -> every 500ms run function

> USART IRQ:
1. upon receiving a byte (data), save it in some global variable THEN send Ack.

> Scheduled Function:
1. [Alternative --- NOT NEEDED **if USART IRQ is used**] wait until data byte is received THEN send Ack.
1. Extract data from received byte.
1. If ID is the same as old ID &rarr; stop motor, light up a red LED, return [This makes MCU check for received byte each time Scheduled function is invoked to resume immediately after connection is back].
1. Else, Drive motor with specified speed and direction + light up a green LED.

## Tasks Distribution

This part is not relevant anymore but it will still be left here for reference.

### Tasks --- Java:
- [x] SerialCommManager class for handling USART communication periodically.  **A**
- [x] GUI for Required Scene. **R**
- [x] Slider handler for required scene. **O**
- [x] TextField for speed + dir. **R**
- [x] App integration with SerialCommManager. **A** 
- <s>[ ] TextField for acknowledgement. **R**</s>
- [x] EXTRA: GUI for Air Conditioner
- [x] EXTRA: Handlers for Knob + Toggle Switch

### Tasks --- MCU Motor Control:
- [x] USART Driver. **O**
- [x] PWM Driver. **R**
- [x] Scheduler (Timer) Driver. 
- [x] DIO/GPIO Driver for LEDs. 
- [x] USART IRQ that handles data reception && ack byte transmission. **O**
- [x] Scheduled function that does the speed control logic. **A**

## Video Demo


https://github.com/user-attachments/assets/43211ef6-6182-4107-b9ca-c02f2d543831


