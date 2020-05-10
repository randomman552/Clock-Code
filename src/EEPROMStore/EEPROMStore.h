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

//There is not much point in abstracting the EEPROM commands any further.
//So we only define the storage locations in this file

#endif