#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

#define FL_MOTOR 3
#define FR_MOTOR 5
#define BR_MOTOR 6
#define BL_MOTOR 9

void setupMotors();
void calibrateMotors();
void stopMotors();

#endif
