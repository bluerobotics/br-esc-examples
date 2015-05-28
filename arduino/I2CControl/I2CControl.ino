/* Blue Robotics Example Code
-------------------------------
 
Title: BlueESC Control via I2C (Arduino)

Description: This example code demonstrates the I2C communication capability
of the Blue Robotics "BlueESC". Motor speed commands are sent via I2C and 
voltage, current, rpm, and temperature data is returned.

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

// See here for example: https://github.com/balrog-kun/autopilot/blob/lpc1343/actuators-i2c.h

#include <Wire.h>

#define I2C_ADDR 0x29

// resistance at 25 degrees C
#define THERMISTORNOMINAL 10000      
// temp. for nominal resistance (almost always 25 C)
#define TEMPERATURENOMINAL 25   
// The beta coefficient of the thermistor (usually 3000-4000)
#define BCOEFFICIENT 3900
// the value of the 'other' resistor
#define SERIESRESISTOR 3300    

// Read the incoming data buffer from an ESC
void readBuffer(uint8_t address, uint8_t buffer[]) {
  Wire.beginTransmission(address);
  Wire.write(0x02); // Data start register
  Wire.endTransmission();
  
  delay(100);
    
  Wire.requestFrom(address,uint8_t(9));
  delay(100);
  uint8_t i = 0;
  while(Wire.available()) {
    buffer[i] = Wire.read();
    i++;
  }
}

// Check to make sure that ESC is properly attached
void initESC(uint8_t address) {
  uint8_t buffer[9];

  delay(100); // Wait for ESCs to boot if starting up
  
  buffer[8] = 0x00;
  readBuffer(address,buffer);
  
  if ( buffer[8] == 0xab ) {
    Serial.println("ESC is alive!");
  } else {
    Serial.println("Can't contact ESC.");
  }
}

// Send motor speed command to ESC
void setESC(uint8_t address, int16_t value) {  
  uint16_t output;

  if ( value < 0 ) {
    output = (-value)|((1<<15)&0x8000);
  } else {
    output = value;
  }

  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.write(output>>8);
  Wire.write(output);
  Wire.endTransmission();
}

void readSensors(uint8_t address, uint16_t *rpm, uint16_t *vbat, uint16_t *temp, uint16_t *curr) {
  uint8_t buffer[9];
  
  readBuffer(address,buffer);
  
  *rpm = (buffer[0] << 8) | buffer[1];
  *vbat = (buffer[2] << 8) | buffer[3];
  *temp = (buffer[4] << 8) | buffer[5];
  *curr = (buffer[6] << 8) | buffer[7];
}

void setup() {
  Wire.begin();
  Serial.begin(19200);
  Serial.println("Starting");
  
  initESC(I2C_ADDR);
}

void loop() {
  static uint32_t writeTimer = 0;
  static uint32_t readTimer = 0;
  
  static int16_t signal = 0;
  
  if ( millis() - writeTimer > 100 ) {
    writeTimer = millis();
  
    static int16_t increment = 200;
    
    if (Serial.available()) {
      signal = Serial.parseInt();
    }
    
    //Serial.print("Sending ");Serial.print(signal);
    setESC(I2C_ADDR,signal);
    
    /*signal += increment;
    
    if ( abs(signal) > 32500 ) {
      increment = -increment;
    }*/
  }
  
  if ( micros() - readTimer > 100000 ) {
    uint32_t dt = micros() - readTimer;
    
    readTimer = micros();
    
    uint16_t rpm, vbat, temp, curr;
    
    readSensors(I2C_ADDR,&rpm,&vbat,&temp,&curr);
    
    float resistance = SERIESRESISTOR/(65535/float(temp)-1);
    
    float steinhart;
    steinhart = resistance / THERMISTORNOMINAL;     // (R/Ro)
    steinhart = log(steinhart);                  // ln(R/Ro)
    steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
    steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart;                 // Invert
    steinhart -= 273.15;                         // convert to C
    
    static float current;
    
    current = current*0.9 + (float(curr)-33344)/65535.0f*5*14.706*0.1;
    
    Serial.write(27);       // ESC command
    Serial.print("[2J");    // clear screen command
    Serial.write(27);
    Serial.print("[H");     // cursor to home command    
    Serial.print("Thro: ");Serial.println(signal);
    Serial.print("RPM:  ");Serial.println(int(rpm/(dt/1000000.0f)*60/6));
    Serial.print("VBat: ");Serial.println(float(vbat)/65536*5*6.45);
    Serial.print("Temp: ");Serial.println(steinhart);
    Serial.print("Curr: ");Serial.println(current);
    Serial.print("Powr: ");Serial.println(current*float(vbat)/65536*5*6.45);       
    Serial.println("---------------------");
  }
}
