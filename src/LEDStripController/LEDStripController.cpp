#include <Arduino.h>
#include "LEDStripController.h"

#pragma region constructors
//TODO: Update the constructors to use templates, similar to how it is done in the default FastLED library.
/* 
* leds (CRGB[]) - The created list of LEDs to work on.
* numLEDs (int) - The number of LEDs.
* brightness (int) - The default brightness to set.
* effect (int) - The default effect to show.
* delay (int) - The default duration of an LED effect (in ms).
* color (CRGB) - The default custom color.
*/
LEDStripHandler::LEDStripHandler(CRGB leds[], int numLEDs, int brightness, int effect, int delay, CRGB color)
{
    LEDStripHandler();
    _leds = leds;
    _numLEDs = numLEDs;
    setBrightness(brightness);
    setDelay(delay);
    setRGB(color);
    setEffect(effect);
}

/*
* leds (CRGB[]) - The created list of LEDs to work on.
* numLEDs (int) - The number of LEDs.
* Other attributes (brightness etc) will be given default values.
*/
LEDStripHandler::LEDStripHandler(CRGB leds[], int numLEDs)
{
    LEDStripHandler(leds, numLEDs, 32, 0, 1000, CRGB(255, 255, 255));
}

/* 
* Empty constructor used to create the object before initalising it.
* This just sets the maximum power draw.
*/
LEDStripHandler::LEDStripHandler()
{
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
}
#pragma endregion

#pragma region Setters and Getters
/*
* Update the effect in use on the LED Strip.
* effect (int) - The new value.
*/
void LEDStripHandler::setEffect(int effect)
{
    _effect = effect;
}

/*
* Get the current effect value.
* return (int) - The internal value of effect.
*/
int LEDStripHandler::getEffect()
{
    return _effect;
}

/*
* Update the brightness value.
* brightness (int) - THe new value.
*/
void LEDStripHandler::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    FastLED.setBrightness(_brightness);
}

/*
* Update the custom color.
* red (uint8) - The new value for red.
* green (uint8) - The new value for green.
* blue (uint8) - The new value for blue.
*/
void LEDStripHandler::setRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    CRGB color(red, green, blue);
    setRGB(color);
}

/*
* Update the custom color.
* color (CRGB) - The new color.
*/
void LEDStripHandler::setRGB(CRGB color)
{
    _color = color;
}

/*
* Update the delay (period of time any given LEDfx will take).
* delay (int) - The new delay value (in milliseconds).
*/
void LEDStripHandler::setDelay(int delay)
{
    _delay = delay;
}
#pragma endregion

#pragma region LED comamnds

/*
* Displays the currently selected effect on the LED strip.
* Any effect will block for the given delay time (set with the setDelay method).
*/
void LEDStripHandler::LEDfx()
{
    switch (_effect)
    {
    case 0:
        _solidFill(_color);
        delay(_delay);
        break;
    default:
        break;
    }
}

/*
* Clear the LED Strip
*/
void LEDStripHandler::_clear()
{
    CRGB color(0, 0, 0);
    _solidFill(color);
}

void LEDStripHandler::_solidFill(CRGB color)
{
    fill_solid(_leds, _numLEDs, color);
    FastLED.show();
}
#pragma endregion