
//Include Libraries
#include <Servo.h>
#include <PPMReader.h>

int interruptPin = 2;
int channelAmount = 6;
PPMReader ppm(interruptPin, channelAmount);

// Channel 1: Roll
// Channel 2: Pitch
// Channel 3: Throttle
// Channel 4: Yaw
// Channel 5: VRA
// Channel 6: VRB

// ALL VALUES FROM 1000 TO 2000 (MID POINT - 1500)

////////////////////////////////
///          Motors          ///
////////////////////////////////

Servo FL_MOTOR;
Servo FR_MOTOR;
Servo BR_MOTOR;
Servo BL_MOTOR;

#define MOTOR_MIN 1060
#define MOTOR_MAX 1860

bool is_motor_calibration = false;

/////////////////////////////////
///          BATTERY          ///
/////////////////////////////////

// Compensation factor, which is inverse of VBAT = (150k / (150k + 150k))
//#define VBAT_DIVIDER_COMP ((33.0 + 100.0) / 33.0)
//#define BATTERY_VOLTAGE_REFERENCE_VALUE 1100
//#define RESOLUTION_STEPS 1023
//#define REAL_BATTERY_MV_PER_LSB (VBAT_DIVIDER_COMP * BATTERY_VOLTAGE_REFERENCE_VALUE / RESOLUTION_STEPS)
//#define BATTERY_PIN A0

//////////////////////////////
///          MAIN          ///
//////////////////////////////

void setup()
{
  FL_MOTOR.attach(3);
  FR_MOTOR.attach(5);
  BR_MOTOR.attach(6);
  BL_MOTOR.attach(9);

  Serial.begin(9600);
  Serial.println("Board Startup");
}

void loop()
{
  


}

/////////////////////////////////
///          BATTERY          ///
/////////////////////////////////

//void get_battery()
//{
//  int rawValue = analogRead(BATTERY_PIN);
//  float voltage = rawValue * REAL_BATTERY_MV_PER_LSB / 1000.0; // Convert to volts
//
//  ackData[0] = voltage; // Assuming ackData is a float array
//}

//////////////////////////////////
///          MOTORS           ////
//////////////////////////////////

void motor_calibration() {

  is_motor_calibration = true;

  FL_MOTOR.writeMicroseconds(MOTOR_MAX);
  FR_MOTOR.writeMicroseconds(MOTOR_MAX);
  BR_MOTOR.writeMicroseconds(MOTOR_MAX);
  BL_MOTOR.writeMicroseconds(MOTOR_MAX);

  delay(2000);
  
  FL_MOTOR.writeMicroseconds(MOTOR_MIN);
  FR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BL_MOTOR.writeMicroseconds(MOTOR_MIN);

  for(int i=MOTOR_MIN; i<=MOTOR_MAX; i++)
  {
    FL_MOTOR.writeMicroseconds(i);
    FR_MOTOR.writeMicroseconds(i);
    BR_MOTOR.writeMicroseconds(i);
    BL_MOTOR.writeMicroseconds(i);

    delay(30);
  }
  delay(500);
  for(int i=2000; i>1000; i--){
    FL_MOTOR.writeMicroseconds(i);
    FR_MOTOR.writeMicroseconds(i);
    BR_MOTOR.writeMicroseconds(i);
    BL_MOTOR.writeMicroseconds(i);
    delay(30);
  }
  
  FL_MOTOR.writeMicroseconds(MOTOR_MIN);
  FR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BL_MOTOR.writeMicroseconds(MOTOR_MIN);

  Serial.println("Motor calibration Complete!");
  is_motor_calibration = false;


}

void stop_motors()
{
  while(is_motor_calibration)
  {
    FL_MOTOR.writeMicroseconds(MOTOR_MIN);
    FR_MOTOR.writeMicroseconds(MOTOR_MIN);
    BR_MOTOR.writeMicroseconds(MOTOR_MIN);
    BL_MOTOR.writeMicroseconds(MOTOR_MIN);
  }
  FL_MOTOR.writeMicroseconds(MOTOR_MIN);
  FR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BR_MOTOR.writeMicroseconds(MOTOR_MIN);
  BL_MOTOR.writeMicroseconds(MOTOR_MIN);
}