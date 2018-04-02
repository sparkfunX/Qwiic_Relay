/******************************************************************************
  Qwiic Relay Firmware 
  Kevin Kuwata @ SparkX
  March 21, 2018
  https://github.com/sparkfunX/Qwiic_Relay
  Arduino 1.8.5

  This is the firmware on the Qwiic Relay. The default I2C Address is
  0x18;

******************************************************************************/
#include <TinyWire.h> //https://github.com/lucullusTheOnly/TinyWire
#include <EEPROM.h>

#define RELAY_PIN   4
#define SETTING_LOCATION_ADDRESS	1

const float  VERSION = 1.1;
 

volatile byte qwiicRelayAddress  =    0x18; //default


volatile int ReceivedData[2]; //2 byte array to act as a buffer for I2C data.  
volatile byte command = 0x99;
volatile byte address = 0x99;

void setup() {
	
	//Read EEPROM, is it empty (0xFF)? or does it have a value?
	 qwiicRelayAddress =  EEPROM.read(SETTING_LOCATION_ADDRESS);
	if(qwiicRelayAddress == 0xFF){
		//EEPROM has never been written before, use the default address 0x18.
		qwiicRelayAddress = 0x18; //default
	}

	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, LOW);

	TinyWire.begin(qwiicRelayAddress);	
	TinyWire.onReceive(receiveEvent); // register event
    TinyWire.onRequest(onI2CRequest);
}

void loop() {
	
	if(ReceivedData[0] == 0x01){
		digitalWrite(RELAY_PIN, HIGH);
		ReceivedData[0] = 0x99;
	}
	
	if(ReceivedData[0] == 0x00){
		digitalWrite(RELAY_PIN, LOW);
		ReceivedData[0] = 0x99;
	}
	
	if(ReceivedData[0] == 0x03){
// check if valid address, if not valid , ignore.
		if(address > 0x07 && address < 78){
			//valid address, update and save to EEPROM
						
		qwiicRelayAddress = address;	
		
		//save to EEPROM
		EEPROM.write(1, qwiicRelayAddress);
		TinyWire.begin(qwiicRelayAddress);		
		}			
		ReceivedData[0] = 0x99;
		ReceivedData[1] = 0x99;
	}
	
	/*
	if(command == 0x01){
		digitalWrite(RELAY_PIN, HIGH);
		command = 0x99;

	}
	
	if(command == 0x00){
		digitalWrite(RELAY_PIN, LOW);
		command = 0x99;
	}
	
	if(command == 0x03){
// check if valid address, if not valid , ignore.
		if(address > 0x07 && address < 78){
			//valid address, update and save to EEPROM
						
		qwiicRelayAddress = address;	
		
		//save to EEPROM
		EEPROM.write(1, qwiicRelayAddress);
		TinyWire.begin(qwiicRelayAddress);		
		}			
		command = 0x99;
		address = 0x99;
	}
	*/
	
	
	if(command == 0x04 ){ //report back with firmware version
	//	TinyWire.send(VERSION);
	// set a flag so that the next time i do a request I sent 4 bytes (= float) to the master...
	/*
		should be similar to the get status but get status was built when we only sent back status
		now we are deciding the status and the firmware version so we need to make a decision
		that leads me to think we need to set a flag of some sort. 
		
		come back to this fix the other examples first. 
		
		lets make sure the changes I did make work. 
	*/
	}
	
}

/*========================================================*/
//        ISR
/*========================================================*/

// When the master initiates a command and data to slave
//		ie) the master says 0x01, then sends a 1, means command: 0x01 then the slave listens for the next thing, which is the relay state 1
//This is modifying ReceivedData array, which stores all the bytes that a master sent to the slave.

void receiveEvent(int bytesReceived) {

byte count = 0; //count of all the data received from master.

  while(TinyWire.available() > 0){ 
	
	ReceivedData[count] = TinyWire.read();
	count++;
 }

/*	
	byte count = 0; //count of all the data received from master.

  while(TinyWire.available() > 0){ 
	if(count == 0){
	command = TinyWire.read();}
	
	if(count == 1){
	address = TinyWire.read();}
	else{
		TinyWire.read();
	}
	count++;
	
  }//end of the while loop, data available
  
  
  */
}// end of receive ISR



//When the master requests data from the slave, this
  //    ISR is triggered. 

void onI2CRequest() {
	//TODO: add in something to report the address.
	if(digitalRead(RELAY_PIN) == HIGH) TinyWire.send(0x01);
	else{
		TinyWire.send(0x00);
	}
}// end of request ISR
