#ifndef LEDStrip_h
#define LEDStrip_h

//Include the required librarys
#include <Arduino.h>
#include <FastLED.h>

//Class for handling the LED strip.
class LEDStripHandler
{
private:
    CRGB *_leds;
    int _numLEDs;
    int _brightness;
    int _effect;
    int _delay;
    CRGB _color;

    //LED effect functions
    void _clear();
    void _solidFill(CRGB color);

public:
    //Constructors
    LEDStripHandler(CRGB leds[], int numLEDs, int brightness, int effect, int delay, CRGB color);
    LEDStripHandler(CRGB leds[], int numLEDs);
    LEDStripHandler();

    //Setters
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);
    void setRGB(CRGB color);
    void setEffect(int effect);
    void setBrightness(uint8_t brightness);
    void setDelay(int delay);

    //Getters
    int getEffect();

    //LED effect functions
    void LEDfx();
};

#endif