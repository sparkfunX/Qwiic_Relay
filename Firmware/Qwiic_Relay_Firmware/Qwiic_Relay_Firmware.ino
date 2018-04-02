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

const float  version = 1.1;
 

volatile byte qwiicRelayAddress  =    0x18; //default


volatile byte command = 0x99;
volatile byte address = 0x99;

volatile byte versionFlag = 99; //this is the flag for flow control within the requestFrom ISR 
					//determines if you are going to send status or version, changed in main after 
					//receive 0x03 command. 

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
	
	
	
	if(command == 0x04 ){ //report back with firmware version
		versionFlag = 1;
	// set a flag so that the next time i do a request I sent 4 bytes (= float) to the master...
	/*
		should be similar to the get status but get status was built when we only sent back status
		now we are deciding the status and the firmware version so we need to make a decision
		that leads me to think we need to set a flag of some sort. 
		
		come back to this fix the other examples first. 
		
		lets make sure the changes I did make work. 
	*/
	}
	
	/*
	//TODO: this might be necessary and make the master look exactly the same
	// structure as the getFrimwareVersion() in master 
	if(command == 0x05){
		versionFlag = 0;
	}
	*/
	
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
	if(count == 0){
	command = TinyWire.read();
	}
	
	else if(count == 1){
	address = TinyWire.read();
	}
	else{
		TinyWire.read(); //read the data coming in but ignore it.
  }
  	count++;
  }
}// end of receive ISR



//When the master requests data from the slave, this
  //    ISR is triggered. 

void onI2CRequest() {
	
	//not sure if this will work because the master sends 0x04, then the salve enters the RX event isr
	// then goes to main, sets the flag. then if that happens slower than versionFlag we have a race condition. 
	

	if(versionFlag == 1){
	TinyWire.send(version); //tiny.wire doesn't like to send multiple bytes so lets see how this goes. we can always_noconv
	//make the version a number just 1 byte then convert over to TinyCore.
	
	versionFlag = 0; // reset the flag
	}

 else if(versionFlag == 0 ){ 
	if(digitalRead(RELAY_PIN) == HIGH) TinyWire.send(0x01); 
	else{
		TinyWire.send(0x00); 
	}
	versionFlag = 99;
}
//put the else if inside so only 1 thing happens per ISR request. 

	
	/*
		so now that we have the version number [float] we want to send we need to have some flow control.
		
	*/
}// end of request ISR
