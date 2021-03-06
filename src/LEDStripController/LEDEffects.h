#ifndef LEDEffects_h
#define LEDEffects_h

#include "LEDStripController.h"
/*
* Collection of lighting functions for the LED strip.
*/
namespace LEDEffects
{

#pragma region clear functions

    /*
    * Clear the LED Strip.
    * leds (CRGB[]) - The leds to clear.
    * numLEDs (int) - The number of leds to clear.
    * updateDisplay (bool) - If true, this function will call FastLED.show().
    */
    void clear(CRGB *leds, const int numLEDs, const bool updateDisplay)
    {
        CRGB color(0, 0, 0);
        fill_solid(leds, numLEDs, color);

        //We have the option to skip this as
        //it is useful to have a clear function that doesn't cause a delay.
        if (updateDisplay)
        {
            FastLED.show();
        }
    }

    /*
    * Clear the LED Strip (and update the LEDStrip)
    * leds (CRGB[]) - The leds to clear.
    * numLEDs (int) - The number of leds to clear.
    */
    void clear(CRGB *leds, const int numLEDs)
    {
        clear(leds, numLEDs, true);
    }

#pragma endregion

#pragma region custom color functions

    /*
    * Fill the LED Strip with the given color.
    * color (CRGB) - The color to set to.
    * leds (CRGB[]) - The leds to change.
    * numLEDs (int) - The number of leds to change.
    * duration (long) - The duration of this functon.
    */
    template <class colorType>
    void solidFill(const colorType color, CRGB *leds, const int numLEDs, const long duration)
    {
        fill_solid(leds, numLEDs, color);
        FastLED.show();
        delay(duration);
    }

    /*
    * Fill the LED Strip with the given color.
    * color (CRGB, CHSV) - The color to set to.
    * leds (CRGB[]) - The leds to change.
    * numLEDs (int) - The number of leds to change.
    */
    template <class colorType>
    void solidFill(const colorType color, CRGB *leds, const int numLEDs)
    {
        solidFill(color, leds, numLEDs, 0);
    }

    /*
    * Fade the LED Strip between black and the given color, then back again.
    * color (CRGB) - The color to fade to and from.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The period of time for this function to take.
    */
    void solidFade(const CRGB color, CRGB *leds, const int numLEDs, const long duration)
    {
        //All functions that fade have similar structure to this.
        //This is the time to wait before and after the fade.
        const long betweenLoopDelay = duration / 4;
        //This is the time to wait between each color change.
        const long inLoopDelay = betweenLoopDelay / 255;

        //How much to shift the color for each interation
        const uint8_t redShift = color.red / 255;
        const uint8_t greenShift = color.green / 255;
        const uint8_t blueShift = color.blue / 255;

        //Fade the color in
        for (int i = 0; i <= 255; i++)
        {
            CRGB newColor(redShift * i, greenShift * i, blueShift * i);
            solidFill(newColor, leds, numLEDs);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);

        //Fade the color out
        for (int i = 255; i >= 0; i--)
        {
            CRGB newColor(redShift * i, greenShift * i, blueShift * i);
            solidFill(newColor, leds, numLEDs);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);
    }

    /*
    * Fade the LED Strip between black and the given color, then back again.
    * color (CHSV) - The color to fade to and from.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The period of time for this function to take.
    */
    void solidFade(const CHSV color, CRGB *leds, const int numLEDs, const long duration)
    {
        const int betweenLoopDelay = duration / 4;
        const int inLoopDelay = betweenLoopDelay / 255;

        const uint8_t valueShift = color.value / 255;

        for (int i = 0; i <= 255; i++)
        {
            const CHSV newColor(color.hue, color.saturation, valueShift * i);
            solidFill(newColor, leds, numLEDs);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);

        for (int i = 255; i >= 0; i--)
        {
            CHSV newColor(color.hue, color.saturation, valueShift * i);
            solidFill(newColor, leds, numLEDs);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);
    }

    /*
    * Fill the LED strip with the custom color, then empty it.
    * color (CRGB) - The color to use.
    * leds (CRGB []) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - How long this function should block for
    */
    template <class colorType>
    void solidFillEmpty(const colorType color, CRGB *leds, const int numLEDs, const long duration)
    {
        const int betweenLoopDelay = duration / 4;
        const int inLoopDelay = betweenLoopDelay / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            solidFill(color, leds, i);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);

        for (int i = numLEDs; i >= 0; i--)
        {
            clear(leds + i, numLEDs - i, true);
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);
    }

    /*
    * LED bounces from one side of the strip to the other.
    * color (CRGB) - The color to use.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The duration of this effect.
    */
    template <class colorType>
    void solidBounce(const colorType color, CRGB *leds, const int numLEDs, const long duration)
    {
        const int inLoopDelay = (duration / 2) / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            solidFill(color, leds + i, 1);
            clear(leds + i, 1, false);
            delay(inLoopDelay);
        }
        for (int i = numLEDs; i >= 0; i--)
        {
            solidFill(color, leds + i, 1);
            clear(leds + i, 1, false);
            delay(inLoopDelay);
        }
    }

#pragma endregion

#pragma region random color functions

    /*
    * Fill the LED strip with a random color.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The duration for this function to block for.
    */
    void randomFill(CRGB *leds, const int numLEDs, const long duration)
    {
        CHSV color(random8(), 255, 255);
        solidFill(color, leds, numLEDs, duration);
    }

    /*
    * Fill the LED strip with a random color for each LED.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The duration for this function to block for.
    */
    void randomSingleFill(CRGB *leds, const int numLEDs, const long duration)
    {
        for (int i = 0; i < numLEDs; i++)
        {
            fill_solid(leds + i, 1, CHSV(random8(), 255, 255));
        }
        FastLED.show();
        delay(duration);
    }

    /*
    * Fade to a random color from black, then back to black.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The duration for this function to block for.
    */
    void randomFade(CRGB *leds, const int numLEDs, const long duration)
    {
        CHSV color(random8(), 255, 255);
        solidFade(color, leds, numLEDs, duration);
    }

    /*
    * Fill the LED strip with the custom color, then empty it.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (long) - The duration for this function to block for.
    */
    void randomFillEmpty(CRGB *leds, const int numLEDs, const long duration)
    {
        CHSV color(random8(), 255, 255);
        solidFillEmpty(color, leds, numLEDs, duration);
    }

    /*
    * Random LED color bounces from one side of the strip to the other.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The duration of this effect.
    */
    void randomBounce(CRGB *leds, const int numLEDs, const long duration)
    {
        CHSV color(random8(), 255, 255);
        solidBounce(color, leds, numLEDs, duration);
    }

#pragma endregion

#pragma region rainbow functions

    /*
    * Fill the LED Strip with a rainbow gradient.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The period of time this function should take.
    */
    void rainbowFill(CRGB *leds, const int numLEDs, const long duration)
    {
        const int hueChange = 255 / numLEDs;
        fill_rainbow(leds, numLEDs, 0, hueChange);
        FastLED.show();
        delay(duration);
    }

    /*
    * Shift the LED strip HSV value.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The period of time this function should take.
    */
    void rainbowShift(CRGB *leds, const int numLEDs, const long duration)
    {
        //Calculate delay length
        const int inLoopDelay = duration / 255;

        //Set starting value for hue
        int hue = 0;
        for (int i = 0; i <= 255; i++)
        {
            solidFill(CHSV(hue, 255, 255), leds, numLEDs);
            hue += 1;
            delay(inLoopDelay);
        }
    }

    /*
    * Shift the HSV value of each LED in the LED strip individually.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The period of time this function should take.
    */
    void rainbowSingleShift(CRGB *leds, const int numLEDs, const long duration)
    {
        const int inLoopDelay = duration / 255;

        /*
        * Create an array to store the hue value of each LED
        * This uses quite a bit of memory, 
        * But as the LEDS are of type CRGB, it is not easy to obtain their hue value.
        * And this works well enough
        */
        int hues[numLEDs] = {0};
        const int hueChange = 255 / numLEDs;

        //Set inital hue values.
        for (int i = 0; i < numLEDs; i++)
        {
            hues[i] += hueChange * i;
        }

        for (int i = 0; i <= 255; i++)
        {
            //Change the hue value of each LED
            for (int j = 0; j < numLEDs; j++)
            {
                //Get the hue value, add i and mod it by 255 to keep it in valid range.
                int hue = (hues[j] + i) % 255;

                fill_solid(leds + j, 1, CHSV(hue, 255, 255));
            }
            FastLED.show();
            delay(inLoopDelay);
        }
    }

    /*
    * Fill the LED strip with a rainbow gradient, then empty it again.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (long) - The period of time this function should take.
    */
    void rainbowFillEmpty(CRGB *leds, const int numLEDs, const long duration)
    {
        const int betweenLoopDelay = duration / 4;
        const int inLoopDelay = betweenLoopDelay / numLEDs;

        const int hueChange = 255 / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            fill_rainbow(leds, i, 0, hueChange);
            FastLED.show();
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);

        for (int i = numLEDs; i >= 0; i--)
        {
            clear(leds + i, numLEDs - i, false);
            FastLED.show();
            delay(inLoopDelay);
        }

        delay(betweenLoopDelay);
    }

#pragma endregion

} // namespace LEDEffects

#endif