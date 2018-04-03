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

enum{
  ERROR_I2C_FAILED,
  ERROR_INVALID_I2C_ADDRESS
};

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Master Awake");

  Wire.begin(); // join the I2C Bus

  Serial.print("Qwiic relay firmware version: ");
  Serial.println(readFirmwareVersion(), 1);
}

void loop() {

}

float readFirmwareVersion() {
  Wire.beginTransmission(Qwiic_Relay_Address); // transmit to device 0x18
  Wire.write(0x04); //Command to check version
  Wire.endTransmission();     // stop transmitting

  Wire.requestFrom(Qwiic_Relay_Address, 4);

  unsigned long versionNum = 0;
  byte counter = 0;
  while(Wire.available() > 4)
  {
    if(counter++ < 4)
    {
      versionNum |= Wire.read();
      versionNum <<= 8;
    }
  }
  return(versionNum);
}

