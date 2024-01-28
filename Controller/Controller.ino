//Include Libraries
#include <string.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

String mesgDictionary[] = {
  "placeholder",
  "Check Connection",
  "Motor Calibration",
  "Stop Motors",
  "Get Battery",
};

//create an RF24 object
RF24 radio(9, 8);  // CE, CSN
const byte address[5] = {'R','x','A','A','A'};
int ackData[2] = {-1, -1};
//bool newAck = false;

// Max size of this struct is 32 bytes - NRF24L01 buffer limit
struct Data_Package {
  byte joy1_X;
  byte joy1_Y;
  byte j1Button;
  byte joy2_X;
  byte joy2_Y;
  byte j2Button;
  byte pot1;
  byte pot2;
  byte tSwitch1;
  byte tSwitch2;
  byte stopMotors;
  byte calibrateMotors;
  byte getbattery;
  byte button4;
  byte pitch;
  byte roll;

};
Data_Package radio_data; //Create a variable with the above structure

struct Ack_Package {
  byte batteryVoltage;
  byte timeSignature;
};
//Ack_Package ackData;




void setup()
{
  Serial.begin(9600);

  if (!radio.begin()) {
    Serial.println(F("radio hardware is not responding!!"));
    while (1) {}  // hold in infinite loop
  }
  radio.openWritingPipe(address);
  radio.enableAckPayload();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.setRetries(5,5); // delay, count
  //5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload

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

  Serial.println("Setup OK");
}

void loop()
{
  bool sendRadio = false;
  
  if (Serial.available())
  { 
    int msg = Serial.readString().toInt();
    String translatedMsg = mesgDictionary[msg];

    Serial.println("Arduino Output: " + translatedMsg);
    
    if(translatedMsg == "Motor Calibration")
    {
      radio_data.calibrateMotors = 2;
      sendRadio = true;
    }
    else if(translatedMsg == "Stop Motors")
    {
      radio_data.stopMotors = 2;
      sendRadio = true;
    }

    if(translatedMsg == "Get Battery")
    {
      radio_data.getbattery = 2;
      sendRadio = true;
    }

  }

  if (sendRadio)
  {
    radio_sendMsg();
  }

  resetData();
}

void radio_sendMsg()
{
  bool rslt = radio.write(&radio_data, sizeof(Data_Package));
  Serial.print("Data Sent ");
  if (rslt)
  {
    if (radio.isAckPayloadAvailable())
    {
      radio.read(&ackData, sizeof(ackData));

      showAck();
    }
    Serial.println("  Acknowledge received");
  }
  else {
    Serial.println("  Tx failed");
  }
}

void showAck()
{
  //Serial.println("Battery Voltage: " + ackData.batteryVoltage);
  //Serial.println("ACK Signature: " + ackData.timeSignature);
  Serial.println("Data: ");
  Serial.print(ackData[0]);
  Serial.println(ackData[1]);
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