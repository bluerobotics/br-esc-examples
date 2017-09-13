Analog Input to ESC Output
==========================

## Description

This example provides code that translates an analog input to a ESC command. This can be used to provide compatibility with controllers that output analog signals or to control the speed directly with a potentiometer or other analog device. It is configured to use a logical input (+5V or 0V) to determine rotation direction. 

The code is written to work with Blue Robotics Basic ESC and BlueESC, however it can be adapted for use with any ESC.

## Wiring

1. Connect analog input signals to A0 and A1 on the Arduino.

2. Connect direction input signals to digital pins 2 and 3, respectively.

3. The ESC signal cables have three wires, brown is ground and orange is the signal. We do not need the center wire. Using jumper wires, connect the brown ground pins to the Arduino *GND* pins (through breadboard if necessary).

4. Connect using wire jumpers:

  * First thruster ESC signal pin to Arduino pin *9* (controlled by A0 and pin 2)
  * Second thruster ESC signal pin to Arduino pin *10* (controlled by A1 and pin 3)

6. Supply power to the ESCs via the red and black cables.
