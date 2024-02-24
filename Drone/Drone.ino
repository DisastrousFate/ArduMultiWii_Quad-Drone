//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "motor.h"
#include "radio.h"

#define BATTERY_PIN A0
#define VBAT_DIVIDER_COMP ((33.0 + 100.0) / 33.0)
#define BATTERY_VOLTAGE_REFERENCE_VALUE 1100
#define RESOLUTION_STEPS 1023
#define REAL_BATTERY_MV_PER_LSB (VBAT_DIVIDER_COMP * BATTERY_VOLTAGE_REFERENCE_VALUE / RESOLUTION_STEPS)

bool is_motor_calibration = false;
int lastTime = 1;

unsigned long lastReceiveTime = 0;
unsigned long currentTime = 0;

void setup()
{
  // Set internal 1.1V voltage reference
  analogReference(INTERNAL);

  Serial.begin(9600);
  Serial.println("Board Startup");

  setupMotors();

  setupRadio();
}

void loop()
{
  bool sendAck = false;

  radio_data = checkRadio();
  if (sizeof(radio_data) == sizeof(Data_Package)) {
    // radio_data exists

    lastReceiveTime = millis();

    int int_calibrateMotors = radio_data.calibrateMotors;
    if (int_calibrateMotors == 2)
    {
      Serial.println("Calibrate Motors");
      calibrateMotors();
    }

    int int_stopMotors = radio_data.stopMotors;
    if (int_stopMotors == 2)
    {
      Serial.println("Stop Motors");
      stopMotors();
    }

    int int_getBattery = radio_data.getbattery;
    if (int_getBattery == 2)
    {
      Serial.println("Get Battery");
      get_battery();
      sendAck = true;
    }
  }

  if (sendAck == true)
  {
    send_Ack();
  }

  currentTime = millis();
  if(currentTime - lastReceiveTime > 1000)
  {
    resetData();
  }
  
}

void get_battery()
{
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = rawValue * REAL_BATTERY_MV_PER_LSB / 1000.0; // Convert to volts

  ackData[0] = voltage; // Assuming ackData is a float array
}
