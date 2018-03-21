/******************************************************************************
  Qwiic Relay Example 1 - Basic Control
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This sketch controls the Qwiic Relay breakout over I2C and demonstrates
  the basic functionality.


******************************************************************************/

#include <Wire.h>

byte LATEST_ADDRESS = 0x18;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Example 1 Basic Control");
  Wire.begin(); // join the I2C Bus
}

void loop() {
  relayOn();    // Turn the Relay On
  Serial.println("Relay is On.");
  delay(2000);  // Wait 2 Seconds
  relayOff();   // Turn the Relay Off
  Serial.println("Relay is Off.");
  delay(2000);  // Wait 2 Seconds
}

// RelayOn() turns on the relay at the LATEST_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(LATEST_ADDRESS); // transmit to default address: 0x18
  Wire.write(0x01);           // Writes 0x01 to the Slave
  Wire.endTransmission();     // stop transmitting
}


// RelayOff() turns off the relay at the LATEST_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(LATEST_ADDRESS); // transmit to device 0x18
  Wire.beginTransmission(LATEST_ADDRESS); // transmit to device 0x18
  Wire.write(0x00);              // Writes 0x00 to the Slave
  Wire.endTransmission();       // stop transmitting
}
