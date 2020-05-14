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
    LEDStripHandler(int brightness, int effect, int delay, CRGB color);

    /*
    * Initalises the LEDStrip with default values.
    */
    LEDStripHandler() : LEDStripHandler(100, 0, 1000, CRGB(255, 255, 255)){};

    /*
    * Init function.
    * leds (CRGB[]) - The leds to assign to the controller.
    * numLEDs (int) - The number of leds.
    */
    void init(CRGB leds[], int numLEDs);

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
};

#endif