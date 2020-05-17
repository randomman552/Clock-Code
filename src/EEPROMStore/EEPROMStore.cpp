#include "EEPROMStore.h"

void updateEEPROMStore(uint8_t effect, CRGB color, uint8_t brightness)
{
    EEPROM.update(EFFECT_STORE, effect);
    EEPROM.update(RED_STORE, color.red);
    EEPROM.update(GREEN_STORE, color.green);
    EEPROM.update(BLUE_STORE, color.blue);
    EEPROM.update(BRIGHTNESS_STORE, brightness);
}

void updateAlarmTime(uint8_t alarmHour, uint8_t alarmMinute)
{
    EEPROM.update(ALARM_HOUR_STORE, alarmHour);
    EEPROM.update(ALARM_MINUTE_STORE, alarmMinute);
}