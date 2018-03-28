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
}

void loop() {
  relayOn(); // Turn on the Relay (at address 0x18)
  delay(1000);
  byte error = changeAddress(0x19); // Change the Relay's address to 0x19
  delay(100);   //Allow the slave time to change it's address.
  relayOff(); // Turn off the Relay (at address 0x19)
  delay(1000);
  changeAddress(0x00);
  if (error) { // Try to change address to Invalid Address.
    Serial.println("!!!!! invalid address" );
  }
  else if (error == 0) {
    Serial.println("success");
  }
delay(1000);
}


// changeAddress() takes a 7 bit I2C Address
// and writes it to the Relay. This function
// checks to see if the address is between
// 0x07 and 0x78. If valid, the new address is
// saved to the Relay's EEPROM. If not valid
// address is not changed and is ignored.
// This function returns 0 if succesful and
// -1 if unsuccessful.
byte changeAddress(byte _address) {
  //check here for an ACK from the slave
  //if Wire.endTransmission() returns a 2, Slave not found.
  if (Wire.endTransmission() == 2) {
    Serial.println("Check Connections. No slave found.");
    return -1;
  }
  else {
    Wire.beginTransmission(Qwiic_Relay_Address);
  }
  //check if valid address.
  if (_address > 0x07 && _address < 0x78) {
    Serial.print("The current address is: ");
    Serial.println(Qwiic_Relay_Address, HEX);

    //valid address
    Wire.beginTransmission(Qwiic_Relay_Address); // transmit to device

    Wire.write(0x03);        // writes the change address command
    Qwiic_Relay_Address = _address;
    Wire.write(Qwiic_Relay_Address);              // sends new address
    Wire.endTransmission();    // stop transmitting
    Wire.begin(Qwiic_Relay_Address);// start with the new address..
    Serial.print("The new address is: 0x");
    Serial.println(_address, HEX);
  }
  else {
    Serial.print("The address 0x");
    Serial.print(_address);
    Serial.println(" is an invalid I2C Address. Needs to be between 0x77 and 0x78.");
    Serial.print("The address is still: 0x");
    Serial.println(Qwiic_Relay_Address, HEX);
    return -1;
  }
  return 0;
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
