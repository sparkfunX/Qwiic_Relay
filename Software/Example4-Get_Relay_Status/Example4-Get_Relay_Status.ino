/******************************************************************************
  Qwiic Relay Example 2 - Change I2C Address
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This sketch demonstrates how to get the current status of the 
  of the Qwiic Relay. The master requests 1 byte of data from the Relay and the 
  Relay responds with a 1 for on and a 0 for off.

  Default Qwiic relay address is 0x18.

  

******************************************************************************/
#include <Wire.h>

const byte qwiicRelayAddress = 0x18;     //Default Address

void setup() {
  Serial.begin(9600);
  Serial.println("Qwiic Relay Master Awake");
  
  Wire.begin(); // join the I2C Bus
  testForConnectivity();
  
  relayOn();
  delay(1000);
}

void loop() {

  if (getStatus() == 1) {
    Serial.println("Relay is on.");
  }
  else if (getStatus() == 0) {
    Serial.println("Relay is off.");
  }
  else {
    Serial.println("An error occurred.");
  }
  delay(1000);
  relayOff();
  delay(1000);
}

// getStatus() returns the status of the Qwiic Relay.
// if returns a 1 the relay is on, return a 0 if the
// Relay is off, and a -1 if an error occurred.
byte getStatus() {
  Wire.requestFrom(qwiicRelayAddress, 1);    // request 1 bytes from slave device qwiicRelayAddress
  while (Wire.available()) { // slave may send less than requested
    char c = Wire.read(); // receive a byte as character.
    if (c == 0x01) {
      return 1;
    }
    else if (c == 0x00) {
      return 0;
    }
      return -1; //error
  }
}



// RelayOn() turns on the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOn() {
  Wire.beginTransmission(qwiicRelayAddress); 
  Wire.write(0x01);           
  Wire.endTransmission();    
}

// RelayOff() turns off the relay at the SLAVE_ADDRESS
// Checks to see if a slave is connected and prints a
// message to the Serial Monitor if no slave found.
void relayOff() {
  Wire.beginTransmission(qwiicRelayAddress); 
  Wire.write(0x00);             
  Wire.endTransmission();      
}


// testForConnectivity() checks for an ACK from an Relay. If no ACK
// program freezes and notifies user. 
void testForConnectivity(){
	Wire.beginTransmission(qwiicRelayAddress);
	//check here for an ACK from the slave, if no ACK don't allow change?
	if(Wire.endTransmission() != 0){
		Serial.println("Check connections. No slave attached.");
		while(1);
	}
}