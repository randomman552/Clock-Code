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
    /*
    * Clear the LED Strip
    */
    void _clear();

    /*
    * Fill the LED Strip with the given color.
    * color (CRGB) - The color to set to.
    */
    void _solidFill(CRGB color);

public:
    //Constructors
    /* 
    * leds (CRGB[]) - The created list of LEDs to work on.
    * numLEDs (int) - The number of LEDs.
    * brightness (int) - The default brightness to set.
    * effect (int) - The default effect to show.
    * delay (int) - The default duration of an LED effect (in ms).
    * color (CRGB) - The default custom color.
    */
    LEDStripHandler(CRGB leds[], int numLEDs, int brightness, int effect, int delay, CRGB color);

    /*
    * leds (CRGB[]) - The created list of LEDs to work on.
    * numLEDs (int) - The number of LEDs.
    * Other attributes (brightness etc) will be given default values.
    */
    LEDStripHandler(CRGB leds[], int numLEDs);

    /* 
    * Empty constructor used to create the object before initalising it.
    * This just sets the maximum power draw.
    */
    LEDStripHandler();

    //Setters
    /*
    * Update the custom color.
    * red (uint8) - The new value for red.
    * green (uint8) - The new value for green.
    * blue (uint8) - The new value for blue.
    */
    void setRGB(uint8_t red, uint8_t green, uint8_t blue);

    /*
    * Update the custom color.
    * color (CRGB) - The new color.
    */
    void setRGB(CRGB color);

    /*
    * Update the effect in use on the LED Strip.
    * effect (int) - The new value.
    */
    void setEffect(int effect);

    /*
    * Update the brightness value.
    * brightness (int) - THe new value.
    */
    void setBrightness(uint8_t brightness);

    /*
    * Update the delay (period of time any given LEDfx will take).
    * delay (int) - The new delay value (in milliseconds).
    */
    void setDelay(int delay);

    //Getters
    /*
    * Get the current effect value.
    * return (int) - The internal value of effect.
    */
    int getEffect();

    //LED effect functions
    /*
    * Displays the currently selected effect on the LED strip.
    * Any effect will block for the given delay time (set with the setDelay method).
    */
    void LEDfx();
};

#endif