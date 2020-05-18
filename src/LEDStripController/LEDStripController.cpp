#include <Arduino.h>
#include "LEDStripController.h"
#include "LEDEffects.h"

#define MAX_BRIGHTNESS 64.0F
#define MAX_EFFECT 12

#pragma region init function

void LEDStripHandler::init(CRGB *leds, int numLEDs)
{
    _leds = leds;
    _numLEDs = numLEDs;
}

#pragma endregion

#pragma region constructors

LEDStripHandler::LEDStripHandler(int brightness, int effect, int delay, CRGB color)
{
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 1500);
    setBrightness(brightness);
    setDelay(delay);
    setRGB(color);
    setEffect(effect);
}

#pragma endregion

#pragma region Setters and Getters

void LEDStripHandler::setEffect(int effect)
{
    //Check if new effect number is in the valid range
    if (effect >= 0 && effect <= MAX_EFFECT)
    {
        _effect = effect;
    }
}

int LEDStripHandler::getEffect()
{
    return _effect;
}

int LEDStripHandler::getMaxEffect()
{
    return MAX_EFFECT;
}

void LEDStripHandler::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    int newValue = (int)(MAX_BRIGHTNESS * ((float)brightness / 100));
    FastLED.setBrightness(newValue);
}

int LEDStripHandler::getBrightness()
{
    return _brightness;
}

void LEDStripHandler::setRGB(uint8_t red, uint8_t green, uint8_t blue)
{
    CRGB color(red, green, blue);
    setRGB(color);
}

void LEDStripHandler::setRGB(CRGB newColor)
{
    color = newColor;
}

void LEDStripHandler::setDelay(int delay)
{
    _delay = delay;
}

#pragma endregion

#pragma region LED comamnds

void LEDStripHandler::LEDfx()
{
    if (_enabled)
    {
        switch (_effect)
        {
        //Solid color effects
        case 0:
            LEDEffects::solidFill(color, _leds, _numLEDs, _delay);
            break;
        case 1:
            LEDEffects::solidFade(color, _leds, _numLEDs, _delay);
            break;
        case 2:
            LEDEffects::solidFillEmpty(color, _leds, _numLEDs, _delay);
            break;
        case 3:
            LEDEffects::solidBounce(color, _leds, _numLEDs, _delay);
            break;
        //Random effects
        case 4:
            LEDEffects::randomFill(_leds, _numLEDs, _delay);
            break;
        case 5:
            LEDEffects::randomSingleFill(_leds, _numLEDs, _delay);
            break;
        case 6:
            LEDEffects::randomFade(_leds, _numLEDs, _delay);
            break;
        case 7:
            LEDEffects::randomFillEmpty(_leds, _numLEDs, _delay);
            break;
        case 8:
            LEDEffects::randomBounce(_leds, _numLEDs, _delay);
            break;
        //Rainbow effects
        case 9:
            LEDEffects::rainbowFill(_leds, _numLEDs, _delay);
            break;
        case 10:
            LEDEffects::rainbowShift(_leds, _numLEDs, _delay);
            break;
        case 11:
            LEDEffects::rainbowFillEmpty(_leds, _numLEDs, _delay);
            break;
        case 12:
            LEDEffects::rainbowSingleShift(_leds, _numLEDs, _delay);
            break;
        default:
            break;
        }
    }
    else
    {
        LEDEffects::clear(_leds, _numLEDs, true);
        millisDelay(_delay);
    }
}

void LEDStripHandler::toggle()
{
    _enabled = !_enabled;
}

#pragma endregion

//Undefine MAX_BRIGHTNESS so it doesn't interfere with other code
#undef MAX_BRIGHTNESS
#undef MAX_EFFECT