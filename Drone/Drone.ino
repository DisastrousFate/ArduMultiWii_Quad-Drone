
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define FL_MOTOR 3
#define FR_MOTOR 5
#define BR_MOTOR 6
#define BL_MOTOR 9

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN
const byte address[6] = "50401";

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

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
  byte button3;
  byte button4;
  byte pitch;
  byte roll;

};

Data_Package radio_data;

void setup()
{
  Serial.begin(9600);
  Serial.println("Board Startup");

  if (!radio.begin()){
    Serial.println("Radio hardware not responding!!");
    while (1) {}
  }

  radio.openReadingPipe(0, address);
  radio.setAutoAck(true); // Ensure autoACK is enabled
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //Set module as transmitter

  Serial.println(radio.getDataRate());

}
void loop()
{

  if(radio.available())
  {
    Serial.println("radio seen");
    radio.read(&radio_data, sizeof(Data_Package));

    lastReceiveTime = millis();
  }

  currentTime = millis();
  if(currentTime - lastReceiveTime > 1000)
  {
    resetData();
  }

  //Serial.println(radio_data.stopMotors);
  Serial.println(radio_data.calibrateMotors);
  int toint = radio_data.calibrateMotors;
  Serial.println(toint);

  if (toint == 2)
  {
    Serial.println("Calibrate Motors");
    motor_calibration();
  }

  
}

int radio_readMsg()
{ 
  int isAvailable = false;
  if(radio.available())
  {
    Serial.println("radio seen");
    Serial.println("git");
    radio.read(&radio_data, sizeof(Data_Package));

    lastReceiveTime = millis();

    isAvailable = true;
  }

  currentTime = millis();
  if(currentTime - lastReceiveTime > 1000)
  {
    resetData();
  }
  
  return isAvailable;
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
  radio_data.button3 = 1;
  radio_data.button4 = 1;
  radio_data.pitch = 0;
  radio_data.roll = 0;
}

void motor_calibration()
{
  analogWrite(FL_MOTOR, 255);
  analogWrite(FR_MOTOR, 255);
  analogWrite(BR_MOTOR, 255);
  analogWrite(BL_MOTOR, 255);

  delay(2000);
  analogWrite(FL_MOTOR, 0);
  analogWrite(FR_MOTOR, 0);
  analogWrite(BR_MOTOR, 0);
  analogWrite(BL_MOTOR, 0);

  for(int i=0; i<=255; i++)
  {
    analogWrite(FL_MOTOR, i);
    analogWrite(FR_MOTOR, i);
    analogWrite(BR_MOTOR, i);
    analogWrite(BL_MOTOR, i);
    delay(30);
  }
  delay(500);
  for(int i=255; i>0; i--){
    analogWrite(FL_MOTOR, i);
    analogWrite(FR_MOTOR, i);
    analogWrite(BR_MOTOR, i);
    analogWrite(BL_MOTOR, i);
    delay(30);
  }

  Serial.println("Motor calibration Complete!");
}
