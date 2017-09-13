Analog Joystick Example
=======================

## Description

This example provides a simple program for the Arduino microcontroller that reads from an analog joystick and outputs to three thrusters. It is designed for three thrusters, two forward and one vertical, and two [Parallax Joysticks](http://www.parallax.com/product/27808).

## Wiring

1. Plug [Parallax Joystick](http://www.parallax.com/product/27808) interface boards into the breadboard.

2. Using wire jumpers, connect the *Vss* pins to the *5V* pin on the Arduino. Connect the *Vdd* pins to any of the *GND* pins on the Arduino.

3. Connect using wire jumpers:

  * *Vert* on one joystick to Arduino pin *A0*
  * *Horiz* on the same joystick to Arduino pin *A1*
  * *Vert* on the other joystick to Arduino pin *A2*

4. The ESC signal cables have three wires, brown is ground and orange is the signal. We do not need the center wire. Using the wire jumpers, connect the brown ground pins to the Arduino *GND* pins (through breadboard if necessary).

5. Connect using wire jumpers:

  * Left thruster ESC signal pin to Arduino pin *9*
  * Right thruster ESC signal pin to Arduino pin *10*
  * Vertical thruster ESC signal pin to Arduino pin *11*

6. Supply 12V power to the ESCs via the red and black cables.

## Wiring Diagram

This diagram shows how the wires should be connected to the Parallax joystick boards as well as the ESC plugs. Make sure to power the ESCs with 12V also by connecting the black wire to ground (negative) and the red wire to 12V (positive).

![Wiring Diagram](https://raw.githubusercontent.com/bluerobotics/br-esc-examples/master/arduino/AnalogJoystickControl/AnalogJoystickControl-Diagram-rev1.png "Wiring Diagram for Joystick Example")
