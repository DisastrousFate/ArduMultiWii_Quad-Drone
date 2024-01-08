//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

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
  //Send message to receiver
  const char text[] = "Hello World";
  Serial.println(radio.write(&text, sizeof(text)));
  Serial.println("Sent");
  
  delay(1000);
}