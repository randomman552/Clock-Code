#include <Arduino.h>
#include "LEDStripController.h"

#pragma region constructors

//TODO: Update the constructors to use templates, similar to how it is done in the default FastLED library.
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

LEDStripHandler::LEDStripHandler(CRGB leds[], int numLEDs)
{
    LEDStripHandler(leds, numLEDs, 32, 0, 1000, CRGB(255, 255, 255));
}

LEDStripHandler::LEDStripHandler()
{
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
}
#pragma endregion

#pragma region Setters and Getters

void LEDStripHandler::setEffect(int effect)
{
    _effect = effect;
}

int LEDStripHandler::getEffect()
{
    return _effect;
}

void LEDStripHandler::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    FastLED.setBrightness(_brightness);
}

void LEDStripHandler::setRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    CRGB color(red, green, blue);
    setRGB(color);
}

void LEDStripHandler::setRGB(CRGB color)
{
    _color = color;
}

void LEDStripHandler::setDelay(int delay)
{
    _delay = delay;
}
#pragma endregion

#pragma region LED comamnds

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