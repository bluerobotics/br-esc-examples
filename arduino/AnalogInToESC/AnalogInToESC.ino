/* Blue Robotics Example Code
-------------------------------
 
Title: Analog Input to ESC Output (Arduino)

Description: This code is an example of how to control the Blue Robotics 
Thrusters and ESCs with an analog input signal. This can be useful to work 
with devices that output analog signals. The speed is controlled by a 0-5V
analog input and the direction is controlled by a logical input with 0V for 
reverse and 5V for forward. The code is designed to operate two thrusters
but can be extended to operate more.

The code is designed for the Arduino Uno board and can be compiled and 
uploaded via the Arduino 1.0+ software.

-------------------------------
The MIT License (MIT)

Copyright (c) 2015 Blue Robotics Inc.

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

// ESC/Thruster Settings
static const int MAX_FWD_REV_THROTTLE = 400; // Value between 0-400
static const int CENTER_THROTTLE = 1500;

// Arduino Pins
static const byte ANALOG_IN_0 = A0;
static const byte ANALOG_IN_1 = A1;
static const byte DIRECTION_0 = 2;
static const byte DIRECTION_1 = 3;
static const byte THRUSTER_0 = 9;
static const byte THRUSTER_1 = 10;

// Servos
Servo thruster0;
Servo thruster1;

void setup() {
  // Set up serial port to print inputs and outputs
  Serial.begin(38400);

  // Set up direction pins as inputs
  pinMode(DIRECTION_0,INPUT);
  pinMode(DIRECTION_1,INPUT);

  // Set up Arduino pins to send servo signals to ESCs
  thruster0.attach(THRUSTER_0);
  thruster1.attach(THRUSTER_1);

  // Set output signal to 1500 microsecond pulse (stopped command)
  thruster0.writeMicroseconds(CENTER_THROTTLE);
  thruster0.writeMicroseconds(CENTER_THROTTLE);

  // Delay to allow time for ESCs to initialize
  delay(1000); 
}

void loop() {
  // Read analog input and apply direction, negative for reverse
  int input0 = analogRead(ANALOG_IN_0)*(digitalRead(DIRECTION_0)*2-1);
  int input1 = analogRead(ANALOG_IN_1)*(digitalRead(DIRECTION_1)*2-1);

  // Map analog input to a PWM output command given in microseconds
  int output0 = map(input0,-1023,1023,CENTER_THROTTLE-MAX_FWD_REV_THROTTLE,CENTER_THROTTLE+MAX_FWD_REV_THROTTLE);
  int output1 = map(input1,-1023,1023,CENTER_THROTTLE-MAX_FWD_REV_THROTTLE,CENTER_THROTTLE+MAX_FWD_REV_THROTTLE);

  // Output command to ESCs
  thruster0.writeMicroseconds(output0);
  thruster1.writeMicroseconds(output1);

  // Output via serial (for debugging)
  Serial.print("Motor 0: "); Serial.print(output0);
  Serial.print("Motor 1: "); Serial.print(output1);
  Serial.println("");

  // Delay 1/20th of a second. No need to update at super fast rates.
  delay(50);
}
