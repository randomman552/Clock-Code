#ifndef LEDEffects_h
#define LEDEffects_h

#include "LEDStripController.h"
#include "../millisDelay.h"
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
    void clear(CRGB *leds, int numLEDs, bool updateDisplay)
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
    void clear(CRGB *leds, int numLEDs)
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
    * duration (int) - The duration of this functon.
    */
    template <class colorType>
    void solidFill(colorType color, CRGB *leds, int numLEDs, int duration)
    {
        fill_solid(leds, numLEDs, color);
        FastLED.show();
        millisDelay(duration);
    }

    /*
    * Fill the LED Strip with the given color.
    * color (CRGB, CHSV) - The color to set to.
    * leds (CRGB[]) - The leds to change.
    * numLEDs (int) - The number of leds to change.
    */
    template <class colorType>
    void solidFill(colorType color, CRGB *leds, int numLEDs)
    {
        solidFill(color, leds, numLEDs, 0);
    }

    /*
    * Fade the LED Strip between black and the given color, then back again.
    * color (CRGB) - The color to fade to and from.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The period of time for this function to take.
    */
    void solidFade(CRGB color, CRGB *leds, int numLEDs, int duration)
    {
        //All functions that fade have similar structure to this.
        //This is the time to wait before and after the fade.
        const int betweenLoopDelay = duration / 4;
        //This is the time to wait between each color change.
        const int inLoopDelay = betweenLoopDelay / 255;

        //How much to shift the color for each interation
        unsigned int redShift = color.red / 255;
        unsigned int greenShift = color.green / 255;
        unsigned int blueShift = color.blue / 255;

        //Fade the color in
        for (int i = 0; i <= 255; i++)
        {
            CRGB newColor(redShift * i, greenShift * i, blueShift * i);
            solidFill(newColor, leds, numLEDs);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);

        //Fade the color out
        for (int i = 255; i >= 0; i--)
        {
            CRGB newColor(redShift * i, greenShift * i, blueShift * i);
            solidFill(newColor, leds, numLEDs);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);
    }

    /*
    * Fade the LED Strip between black and the given color, then back again.
    * color (CHSV) - The color to fade to and from.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The period of time for this function to take.
    */
    void solidFade(CHSV color, CRGB *leds, int numLEDs, int duration)
    {
        const int betweenLoopDelay = duration / 4;
        const int inLoopDelay = betweenLoopDelay / 255;

        unsigned int valueShift = color.value / 255;

        for (int i = 0; i <= 255; i++)
        {
            CHSV newColor(color.hue, color.saturation, valueShift * i);
            solidFill(newColor, leds, numLEDs);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);

        for (int i = 255; i >= 0; i--)
        {
            CHSV newColor(color.hue, color.saturation, valueShift * i);
            solidFill(newColor, leds, numLEDs);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);
    }

    /*
    * Fill the LED strip with the custom color, then empty it.
    * color (CRGB) - The color to use.
    * leds (CRGB []) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - How long this function should block for
    */
    template <class colorType>
    void solidFillEmpty(colorType color, CRGB *leds, int numLEDs, int duration)
    {
        const int betweenLoopDelay = duration / 4;
        const int inLoopDelay = betweenLoopDelay / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            solidFill(color, leds, i);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);

        for (int i = numLEDs; i >= 0; i--)
        {
            clear(leds + i, numLEDs - i, true);
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);
    }

    /*
    * LED bounces from one side of the strip to the other.
    * color (CRGB) - The color to use.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to effect.
    * duration (int) - The duration of this effect.
    */
    template <class colorType>
    void solidBounce(colorType color, CRGB *leds, int numLEDs, int duration)
    {
        const int inLoopDelay = (duration / 2) / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            solidFill(color, leds + i, 1);
            clear(leds + i, 1, false);
            millisDelay(inLoopDelay);
        }
        for (int i = numLEDs; i >= 0; i--)
        {
            solidFill(color, leds + i, 1);
            clear(leds + i, 1, false);
            millisDelay(inLoopDelay);
        }
    }

#pragma endregion

#pragma region random color functions

    /*
    * Fill the LED strip with a random color.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The duration for this function to block for.
    */
    void randomFill(CRGB *leds, int numLEDs, int duration)
    {
        CHSV color(random8(), 255, 255);
        solidFill(color, leds, numLEDs, duration);
    }

    /*
    * Fill the LED strip with a random color for each LED.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The duration for this function to block for.
    */
    void randomSingleFill(CRGB *leds, int numLEDs, int duration)
    {
        for (int i = 0; i < numLEDs; i++)
        {
            fill_solid(leds + i, 1, CHSV(random8(), 255, 255));
        }
        FastLED.show();
        millisDelay(duration);
    }

    /*
    * Fade to a random color from black, then back to black.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The duration for this function to block for.
    */
    void randomFade(CRGB *leds, int numLEDs, int duration)
    {
        CHSV color(random8(), 255, 255);
        solidFade(color, leds, numLEDs, duration);
    }

    /*
    * Fill the LED strip with the custom color, then empty it.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to act on.
    * duration (int) - The duration for this function to block for.
    */
    void randomFillEmpty(CRGB *leds, int numLEDs, int duration)
    {
        CHSV color(random8(), 255, 255);
        solidFillEmpty(color, leds, numLEDs, duration);
    }

    /*
    * Random LED color bounces from one side of the strip to the other.
    * leds (CRGB[]) - The leds to act on.
    * numLEDs (int) - The number of leds to effect.
    * duration (int) - The duration of this effect.
    */
    void randomBounce(CRGB *leds, int numLEDs, int duration)
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
    * duration (int) - The period of time this function should take.
    */
    void rainbowFill(CRGB *leds, int numLEDs, int duration)
    {
        int hueChange = 255 / numLEDs;
        fill_rainbow(leds, numLEDs, 0, hueChange);
        FastLED.show();
        millisDelay(duration);
    }

    /*
    * Shift the LED strip HSV value.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (int) - The period of time this function should take.
    */
    void rainbowShift(CRGB *leds, int numLEDs, int duration)
    {
        //Calculate delay length
        int inLoopDelay = duration / 255;

        //Set starting value for hue
        int hue = 0;
        for (int i = 0; i <= 255; i++)
        {
            solidFill(CHSV(hue, 255, 255), leds, numLEDs);
            hue += 1;
            millisDelay(inLoopDelay);
        }
    }

    /*
    * Shift the HSV value of each LED in the LED strip individually.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (int) - The period of time this function should take.
    */
    void rainbowSingleShift(CRGB *leds, int numLEDs, int duration)
    {
        int inLoopDelay = duration / 255;

        /*
        * Create an array to store the hue value of each LED
        * This uses quite a bit of memory, 
        * But as the LEDS are of type CRGB, it is not easy to obtain their hue value.
        * And this works well enough
        */
        int hues[numLEDs] = {0};
        int hueChange = 255 / numLEDs;

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
            millisDelay(inLoopDelay);
        }
    }

    /*
    * Fill the LED strip with a rainbow gradient, then empty it again.
    * leds (CRGB[]) - The leds to apply the gradient to.
    * numLEDs (int) - The number of leds to effect.
    * duration (int) - The period of time this function should take.
    */
    void rainbowFillEmpty(CRGB *leds, int numLEDs, int duration)
    {
        int betweenLoopDelay = duration / 4;
        int inLoopDelay = betweenLoopDelay / numLEDs;

        int hueChange = 255 / numLEDs;

        for (int i = 0; i <= numLEDs; i++)
        {
            fill_rainbow(leds, i, 0, hueChange);
            FastLED.show();
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);

        for (int i = numLEDs; i >= 0; i--)
        {
            clear(leds + i, numLEDs - i, false);
            FastLED.show();
            millisDelay(inLoopDelay);
        }

        millisDelay(betweenLoopDelay);
    }

#pragma endregion

} // namespace LEDEffects

#endif