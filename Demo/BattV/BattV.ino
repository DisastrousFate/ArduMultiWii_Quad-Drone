// Compensation factor, which is inverse of VBAT = (150k / (150k + 150k))
#define VBAT_DIVIDER_COMP ((33.0 + 100.0) / 33.0) 
// Interval voltage reference of 1.1V in mV
#define BATTERY_VOLTAGE_REFERENCE_VALUE 1100 
// 10-bit resolution gives 1023 steps
#define RESOLUTION_STEPS 1023
// Combine together from a formula
#define REAL_BATTERY_MV_PER_LSB (VBAT_DIVIDER_COMP * BATTERY_VOLTAGE_REFERENCE_VALUE / RESOLUTION_STEPS)

#define BATTERY_PIN A0

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,  16, 2);

void setup () {
    // Set internal 1.1V voltage reference
    analogReference(INTERNAL); 
    analogWrite(A1, 100);
    Serial.begin(115200);

    //initialize lcd screen
    lcd.init();
    // turn on the backlight
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("hello");
}
void loop () {
    lcd.clear();
    lcd.setCursor(0,0);
    Serial.println(analogRead(BATTERY_PIN) * REAL_BATTERY_MV_PER_LSB);
    lcd.print(analogRead(BATTERY_PIN) * REAL_BATTERY_MV_PER_LSB);
    //Serial.println(analogRead(BATTERY_PIN));
    delay(500);
    
}