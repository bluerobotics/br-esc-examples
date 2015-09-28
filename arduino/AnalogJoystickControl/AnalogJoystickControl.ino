/* Blue Robotics Example Code
-------------------------------
 
Title: Analog Joystick Control Example (Arduino)

Description: This code is an example of how to use the Blue Robotics thrusters 
and ESCs to control a simple underwater vehicle. The code is designed to use 
analog joysticks (http://www.parallax.com/product/27808) and three thrusters. 
The thrusters are oriented with two pointing forward, one on the left and one 
on the right side of the vehicle, as well as a vertical thruster. 

The code is designed for the Arduino Uno board and can be compiled and 
uploaded via the Arduino 1.0+ software.

-------------------------------
The MIT License (MIT)

Copyright (c) 2014 Blue Robotics Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-------------------------------*/

#include <Servo.h>

// Joystick Settings
static const int JS_CENTER_0 = 511; // Analog reading at center, 0-1023
static const int JS_CENTER_1 = 511;
static const int JS_CENTER_2 = 511;
static const int JS_RANGE_0 = 128; // Analog range, 0-1023
static const int JS_RANGE_1 = 128; // Set to 128 for Parallax joystick
static const int JS_RANGE_2 = 128;
static const int JS_DIR_0 = 1; // +1 or -1
static const int JS_DIR_1 = 1;
static const int JS_DIR_2 = 1;

// ESC/Thruster Settings
static const int MAX_FWD_REV_THROTTLE = 400; // Value between 0-400
static const int MAX_TURN_THROTTLE = 400; // Value between 0-400
static const int MAX_VERTICAL_THROTTLE = 400; // Value between 0-400
static const int CENTER_THROTTLE = 1500;

// Arduino Pins
static const byte JS_ADC_0 = A0;
static const byte JS_ADC_1 = A1;
static const byte JS_ADC_2 = A2;
static const byte THRUSTER_LEFT = 9;
static const byte THRUSTER_RIGHT = 10;
static const byte THRUSTER_VERTICAL = 11;

// Servos
Servo thrusterLeft;
Servo thrusterRight;
Servo thrusterVertical;

void setup() {
  // Set up serial port to print inputs and outputs
  Serial.begin(38400);

  // Set up Arduino pins to send servo signals to ESCs
  thrusterLeft.attach(THRUSTER_LEFT);
  thrusterRight.attach(THRUSTER_RIGHT);
  thrusterVertical.attach(THRUSTER_VERTICAL);

  // Set output signal to 1500 microsecond pulse (stopped command)
  thrusterLeft.writeMicroseconds(CENTER_THROTTLE);
  thrusterRight.writeMicroseconds(CENTER_THROTTLE);
  thrusterVertical.writeMicroseconds(CENTER_THROTTLE);

  // Delay to allow time for ESCs to initialize
  delay(1000); 
}

void loop() {
  // Read the joysticks and use the Arduino "map" function to map the raw values
  // to the desired output commands.
  int forwardCommand    = map(analogRead(JS_ADC_0), // Read raw joystick value
                              JS_CENTER_0-JS_DIR_0*JS_RANGE_0, // Joystick low value
                              JS_CENTER_0+JS_DIR_0*JS_RANGE_0, // Joystick high value
                              -MAX_FWD_REV_THROTTLE, // Command low value
                              MAX_FWD_REV_THROTTLE); // Command high value
  int turnCommand       = map(analogRead(JS_ADC_1), // Read raw joystick value
                              JS_CENTER_1-JS_DIR_1*JS_RANGE_1, // Joystick low value
                              JS_CENTER_1+JS_DIR_1*JS_RANGE_1, // Joystick high value
                              -MAX_TURN_THROTTLE, // Command low value
                              MAX_TURN_THROTTLE); // Command high value
  int verticalCommand   = map(analogRead(JS_ADC_2), // Read raw joystick value
                              JS_CENTER_2-JS_DIR_2*JS_RANGE_2, // Joystick low value
                              JS_CENTER_2+JS_DIR_2*JS_RANGE_2, // Joystick high value
                              -MAX_VERTICAL_THROTTLE, // Command low value
                              MAX_VERTICAL_THROTTLE); // Command high value

  // Combine the "stopped" command with forward, turn, and vertical and send 
  // to the ESCs.
  thrusterLeft.writeMicroseconds(CENTER_THROTTLE+forwardCommand+turnCommand);
  thrusterRight.writeMicroseconds(CENTER_THROTTLE+forwardCommand-turnCommand);
  thrusterVertical.writeMicroseconds(CENTER_THROTTLE+verticalCommand);

  // Output via serial
  Serial.print("Fwd: "); Serial.print(forwardCommand);
  Serial.print("Turn: "); Serial.print(turnCommand);
  Serial.print("Vert: "); Serial.print(verticalCommand);
  Serial.println("");

  // Delay 1/10th of a second. No need to update at super fast rates.
  delay(100);
}
