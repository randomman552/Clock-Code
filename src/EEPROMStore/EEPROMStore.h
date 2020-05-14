#ifndef EEPROMStore_h
#define EEPROMStore_h
#include <Arduino.h>
#include <EEPROM.h>

//Define storage locations
#define EFFECT_STORE 0
#define RED_STORE 1
#define GREEN_STORE 2
#define BLUE_STORE 3
#define BRIGHTNESS_STORE 4

//Function to update all of the EEPROM values at once
void updateEEPROMStore(uint8_t effect, CRGB color, uint8_t brightness)
{
    EEPROM.update(EFFECT_STORE, effect);
    EEPROM.update(RED_STORE, color.red);
    EEPROM.update(GREEN_STORE, color.green);
    EEPROM.update(BLUE_STORE, color.blue);
    EEPROM.update(BRIGHTNESS_STORE, brightness);
}

//There is not much point in abstracting the EEPROM commands any further.
//So we only define the storage locations in this file

#endif