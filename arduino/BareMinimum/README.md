Bare Minimum for ESC Control
============================

## Description

This example provides the bare minimum code required to get started with the Blue Robotics Basic ESC or BlueESC and an Arduino microcontroller. The code sets up an Arduino pin to output a PWM (servo) signal and then initializes the ESC with a 1500 microsecond signal. Once initialized, the signal is set to a different value so that the motor will start moving.

## Wiring

*Basic ESC:* The Basic ESC signal cables have three wires, brown is ground and orange is the signal. We do not need the center wire. Using jumper wires, connect the brown ground pins to the Arduino *GND* pins (through breadboard if necessary). Connect the orange signal wire to Arduino pin 9.

*BlueESC:* The BlueESC signal cables have four wires, but all you need is ground (black) and signal (red or yellow). Connect the black ground wire to the Arduino *GND* pins (through breadboard if necessary). Connect the signal wire to Arduino pin 9.

Supply power to the ESCs via the red and black cables.
