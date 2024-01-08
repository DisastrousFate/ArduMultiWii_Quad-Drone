//Include Libraries
#include <string.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

String mesgDictionary[] = {
  "placeholder",
  "Check Connection",
  "Motor Calibration",
  "Stop Motors"
};

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN

//address through which two modules communicate.
const byte address[6] = "50401";

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.setAutoAck(true); // Ensure autoACK is enabled
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();
}
void loop()
{
  readSerial();
  
}

void readSerial()
{
  if (Serial.available() > 0)
    { 
      int msg = Serial.readString().toInt();
      Serial.println(msg);
      String translatedMsg = mesgDictionary[msg];
      Serial.println("Arduino Output: " + translatedMsg);

    }
}

//void radio_sendMsg()
//{
//
//}