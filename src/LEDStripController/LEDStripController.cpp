#include <Arduino.h>
#include "LEDStripController.h"

LEDStripHandler::LEDStripHandler(CRGB leds[])
{
    _leds = leds;
}

LEDStripHandler::LEDStripHandler()
{
}