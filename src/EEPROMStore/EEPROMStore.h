//TODO: Refactor this into a cpp file

#ifndef EEPROMStore_h
#define EEPROMStore_h
#include <Arduino.h>
#include <FastLED.h>
#include <EEPROM.h>

//Define storage locations
#define EFFECT_STORE 0
#define RED_STORE 1
#define GREEN_STORE 2
#define BLUE_STORE 3
#define BRIGHTNESS_STORE 4
#define ALARM_HOUR_STORE 5
#define ALARM_MINUTE_STORE 6

/*Function to update all of the EEPROM values at once*/
void updateEEPROMStore(uint8_t effect, CRGB color, uint8_t brightness);

/*This function updates the alarm time in EEPROM to the given value.*/
void updateAlarmTime(uint8_t alarmHour, uint8_t alarmMinute);

#endif