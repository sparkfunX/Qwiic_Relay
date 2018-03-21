/******************************************************************************
  Qwiic Joystiic Example 1 - Basic Control
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This sketch controls the Qwiic Relay breakout over I2C.
  

******************************************************************************/

#include <Wire.h>

byte SLAVE_ADDRESS = 0x18;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Master Awake");
  Serial.print("Slave Address: 0x");
  Serial.println(SLAVE_ADDRESS, HEX); // Print the current address to the Serial Monitor in hexadecimal.
  Wire.begin(); // join the I2C Bus
}

void loop() {
  relayOn();    // Turn the Relay On
  delay(2000);  // Wait 2 Seconds
  relayOff();   // Turn the Relay Off
  delay(2000);  // Wait 2 Seconds
}

// RelayOn() turns on the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to default address: 0x18
  // Check for an ACK from the slave
  // If Wire.endTransmission() returns 2, No slave was found.

  if (Wire.endTransmission() == 2) {
    Serial.println("Check Connections. Slave not found.");
  }
  else {
    Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device 0x18
    Wire.write(0x01);           // Writes 0x01 to the Slave
    Wire.endTransmission();     // stop transmitting
    Serial.println("Relay is on.");
  }
}


// RelayOff() turns off the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device 0x18
  // Check for an ACK from the slave
  // If Wire.endTransmission() returns 2, No slave was found.
  if (Wire.endTransmission() == 2) {
    Serial.println("Check Connections. Slave not found.");
  }
  else {
    Wire.beginTransmission(SLAVE_ADDRESS); // transmit to device 0x18
    Wire.write(0x00);              // Writes 0x00 to the Slave
    Wire.endTransmission();       // stop transmitting
    Serial.println("Relay is off.");
  }
}
