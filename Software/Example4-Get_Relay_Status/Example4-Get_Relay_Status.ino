/******************************************************************************
  Qwiic Relay Example 2 - Change I2C Address
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This sketch demonstrates how to change the I2C Addres
  of the Qwiic Relay. The new address will be saved to the
  Relay's EEPROM.

  The factory default slave address is 0x18.

  Note: If the Qwiic Relay is connected but unresponsive, try Example 3 - I2C_Scanner
  to find the current address of the Relay.

  Only valid address can be saved to the Relay's EEPROM. If an invalid address is
  sent, the Relay will ignore the address change without notifying the user.

******************************************************************************/
#include <Wire.h>

byte Qwiic_Relay_Address = 0x18;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Master Awake");
  Wire.begin(); // join the I2C Bus

  Wire.beginTransmission(Qwiic_Relay_Address); // transmit to device
  //check here for an ACK from the slave
  //if Wire.endTransmission() returns a 2, slave not found.
  if (Wire.endTransmission() == 2) {
    Serial.println("Check Connections. Slave not found.");
  }
  else {
    Serial.println("Qwiic Relay found!");
  }
  relayOn();
  delay(2000);
}

void loop() {

  if (getStatus() == 1) {
    Serial.println("Relay is on.");
  }
  else if (getStatus() == 0) {
    Serial.println("Relay is off.");
  }
  else {
    Serial.println("an error occured");
  }
  delay(1000);
  relayOff();
  delay(1000);
}

// getStatus() returns the status of the Qwiic Relay.
// if returns a 1 the relay is on, return a 0 if the
// Relay is off, and a -1 if an error occured.
byte getStatus() {
  Wire.requestFrom(Qwiic_Relay_Address, 1);    // request 1 bytes from slave device Qwiic_Relay_Address
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character.
    if (c == 0x01) {
      return 1;
    }
    else if (c == 0x00) {
      return 0;
    }
    else {
      return -1;
    }
  }
}



// RelayOn() turns on the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(Qwiic_Relay_Address); // transmit to device 0x18
  Wire.write(0x01);           // Writes 0x01 to the Slave
  Wire.endTransmission();     // stop transmitting
}

// RelayOff() turns off the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(Qwiic_Relay_Address); // transmit to device 0x18
  Wire.write(0x00);              // Writes 0x00 to the Slave
  Wire.endTransmission();       // stop transmitting
}
