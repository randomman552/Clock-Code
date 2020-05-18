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
    uint8_t _brightness;
    int _effect;
    long _delay;
    volatile bool _enabled = true;

public:
    //Attributes
    /*
    * The custom color used by the LEDStrip handler.
    */
    CRGB color;

    //Constructors
    /* 
    * leds (CRGB[]) - The created list of LEDs to work on.
    * numLEDs (int) - The number of LEDs.
    * brightness (int) - The default brightness to set.
    * effect (int) - The default effect to show.
    * delay (int) - The default duration of an LED effect (in ms).
    * color (CRGB) - The default custom color.
    */
    LEDStripHandler(const uint8_t brightness, const int effect, const long delay, const CRGB color);

    /*
    * Initalises the LEDStrip with default values.
    */
    LEDStripHandler() : LEDStripHandler(100, 0, 1000, CRGB(255, 255, 255)){};

    /*
    * Init function.
    * leds (CRGB[]) - The leds to assign to the controller.
    * numLEDs (int) - The number of leds.
    */
    void init(CRGB leds[], const int numLEDs);

    //Setters
    /*
    * Update the custom color.
    * red (uint8) - The new value for red.
    * green (uint8) - The new value for green.
    * blue (uint8) - The new value for blue.
    */
    void setRGB(const uint8_t red, const uint8_t green, const uint8_t blue);

    /*
    * Update the custom color.
    * newColor (CRGB) - The new color.
    */
    void setRGB(const CRGB newColor);

    /*
    * Update the effect in use on the LED Strip.
    * effect (int) - The new value.
    */
    void setEffect(const int effect);

    /*
    * Update the brightness value.
    * brightness (int) - THe new value.
    */
    void setBrightness(const uint8_t brightness);

    /*
    * Update the delay (period of time any given LEDfx will take).
    * delay (long) - The new delay value (in milliseconds).
    */
    void setDelay(const long delay);

    //Getters
    /*
    * Get the current effect value.
    * return (int) - The internal value of effect.
    */
    int getEffect();

    /*
    * Get the maximum effect value.
    * return (int) - The maximum effect value.
    */
    int getMaxEffect();

    /*
    * Get the current brightness level
    * return (int) - The current brightness level
    */
    int getBrightness();

    //LED effect functions
    /*
    * Displays the currently selected effect on the LED strip.
    * Any effect will block for the given delay time (set with the setDelay method).
    */
    void LEDfx();

    /*
    * Toggle the current state of the LEDStrip.
    * Will disable the LED strip if the brightness is higher than 0.
    * Will set the LED strip back to the correct brightness otherwise.
    */
    void toggle();
};

#endif