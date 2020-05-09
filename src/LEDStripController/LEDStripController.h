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

    //Red value
    uint8_t _r;
    //Green value
    uint8_t _g;
    //Red value
    uint8_t _b;

public:
    //Constructors
    LEDStripHandler(CRGB leds[]);
    LEDStripHandler();

    //Setters
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);
};

#endif