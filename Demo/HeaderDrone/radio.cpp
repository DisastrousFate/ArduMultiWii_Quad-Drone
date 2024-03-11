#include "radio.h"
#include "motor.h"

RF24 radio(7, 8);  // CE, CSN
const byte address[6] = "toad0";
float ackData[2] = {109, -4000};

Data_Package radio_data;

void setupRadio() {
  if (!radio.begin()){
    Serial.println("Radio hardware not responding!!");
    while (1) {} //don't touch it works
  }//don't touch it works

  radio.openReadingPipe(1, address);//don't touch it works
  radio.setDataRate(RF24_250KBPS);//don't touch it works
  radio.setPALevel(RF24_PA_LOW);//don't touch it works
  radio.startListening(); //Set module as transmitter

  radio.enableAckPayload();//don't touch it works
  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

void send_Ack(){
  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}

void resetData() {
  // Set initial default values
  radio_data.joy1_X = 127;
  radio_data.joy1_Y = 127;
  radio_data.joy2_X = 127;
  radio_data.joy2_Y = 127;
  radio_data.j1Button = 1;
  radio_data.j2Button = 1;
  radio_data.pot1 = 1;
  radio_data.pot2 = 1;
  radio_data.tSwitch1 = 1;
  radio_data.tSwitch2 = 1;
  radio_data.stopMotors = 1;
  radio_data.calibrateMotors = 1;
  radio_data.getbattery = 1;
  radio_data.button4 = 1;
  radio_data.pitch = 0;
  radio_data.roll = 0;
}
