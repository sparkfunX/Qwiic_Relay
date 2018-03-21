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

volatile byte SLAVE_ADDRESS  =    0x18; //default


volatile int ReceivedData[32]; //32 byte array to act as a buffer for I2C data. 32 bytes is the max for an UNO 


void setup() {
	
	//Read EEPROM, is it empty (0xFF)? or does it have a value?
	byte value =  EEPROM.read(1);
	if(value == 0xFF){
		//never been written before, USE THE Default address.
		SLAVE_ADDRESS = 0x18; //default
	}
	else{
		SLAVE_ADDRESS = EEPROM.read(1);
	}

	TinyWire.begin(SLAVE_ADDRESS);
    pinMode(RELAY_PIN, OUTPUT);
	TinyWire.onReceive(receiveEvent); // register event
    TinyWire.onRequest(onI2CRequest);
}

void loop() {
	
	if(ReceivedData[0] == 0x01){
		digitalWrite(RELAY_PIN, HIGH);
	}
	if(ReceivedData[0] == 0x00){
		digitalWrite(RELAY_PIN, LOW);
	}
	
	if(ReceivedData[0] == 0x03){
// check if valid address, if not valid , ignore.
		if(ReceivedData[1] > 0x07 && ReceivedData[1] < 78){
			//valid address, update and save to EEPROM
						
		SLAVE_ADDRESS = ReceivedData[1];	
					//save to eerprom
		EEPROM.write(1, SLAVE_ADDRESS);
		TinyWire.begin(SLAVE_ADDRESS);		
		}			
		ReceivedData[0]=0x99;
		ReceivedData[1] = 0x99;
	}
}

/*========================================================*/
//        ISR
/*========================================================*/

// When the master initiates a command and data to slave
//		ie) the master says 0x01, then sends a 1, means command: 0x01 then the slave listens for the next thing, which is the relay state 1
//This is modifying ReceivedData array, which stores all the bytes that a master sent to the slave.

void receiveEvent(int bytesReceived) {
	int index = 0; //index of all the data received from master.
	
  while(TinyWire.available() > 0){ //this was the solution, 
		ReceivedData[index] = TinyWire.read();
		index++;
  }//end of the while loop, data available
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
