
//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define FL_MOTOR 3
#define FR_MOTOR 5
#define BR_MOTOR 6
#define BL_MOTOR 9

// Compensation factor, which is inverse of VBAT = (150k / (150k + 150k))
#define VBAT_DIVIDER_COMP ((33.0 + 100.0) / 33.0)
// Interval voltage reference of 1.1V in mV
#define BATTERY_VOLTAGE_REFERENCE_VALUE 1100
// 10-bit resolution gives 1023 steps
#define RESOLUTION_STEPS 1023
// Combine together from a formula
#define REAL_BATTERY_MV_PER_LSB (VBAT_DIVIDER_COMP * BATTERY_VOLTAGE_REFERENCE_VALUE / RESOLUTION_STEPS)

#define BATTERY_PIN A0

//create an RF24 object
RF24 radio(7, 8);  // CE, CSN
const byte address[6] = "50401";

int ackData[2] = {109,-4000};

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
  byte getbattery;
  byte button4;
  byte pitch;
  byte roll;

};

Data_Package radio_data;

bool is_motor_calibration = false;


void setup()
{
  // Set internal 1.1V voltage reference
  analogReference(INTERNAL);

  Serial.begin(9600);
  Serial.println("Board Startup");

  pinMode(FL_MOTOR, OUTPUT);
  pinMode(FR_MOTOR, OUTPUT);
  pinMode(BR_MOTOR, OUTPUT);
  pinMode(BL_MOTOR, OUTPUT);

  if (!radio.begin()){
    Serial.println("Radio hardware not responding!!");
    while (1) {}
  }

  radio.openReadingPipe(0, address);
  radio.setAutoAck(true); // Ensure autoACK is enabled
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening(); //Set module as transmitter

  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

void loop()
{
  if(radio.available())
  {
    radio.read(&radio_data, sizeof(Data_Package));
    lastReceiveTime = millis();



    int int_calibrateMotors = radio_data.calibrateMotors;
    if (int_calibrateMotors == 2)
    {
      Serial.println("Calibrate Motors");
      motor_calibration();
    }

    int int_stopMotors = radio_data.stopMotors;
    if (int_stopMotors == 2)
    {
      Serial.println("Stop Motors");
      stop_motors();
    }

    int int_getBattery = radio_data.getbattery;
    if (int_getBattery == 2)
    {
      Serial.println("Get Battery");
      get_battery();
    }



  }

  currentTime = millis();
  if(currentTime - lastReceiveTime > 1000)
  {
    resetData();
  }
  
}

void radio_sendAckPayload()
{
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

void motor_calibration()
{
  is_motor_calibration = true;

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

  analogWrite(FL_MOTOR, 0);
  analogWrite(FR_MOTOR, 0);
  analogWrite(BR_MOTOR, 0);
  analogWrite(BL_MOTOR, 0);

  Serial.println("Motor calibration Complete!");
  is_motor_calibration = false;
}

void stop_motors()
{
  while(is_motor_calibration)
  {
    analogWrite(FL_MOTOR, 0);
    analogWrite(FR_MOTOR, 0);
    analogWrite(BR_MOTOR, 0);
    analogWrite(BL_MOTOR, 0);
  }
  analogWrite(FL_MOTOR, 0);
  analogWrite(FR_MOTOR, 0);
  analogWrite(BR_MOTOR, 0);
  analogWrite(BL_MOTOR, 0);
}

void get_battery()
{
  Serial.println(analogRead(BATTERY_PIN) * REAL_BATTERY_MV_PER_LSB);
  radio_sendAckPayload();
}
