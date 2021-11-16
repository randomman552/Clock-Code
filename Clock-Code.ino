#include <LEDStripController.h>
#include "src/Clock.h"

//LED Strip Setup
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 34
CRGB leds[NUM_LEDS];

Clock clock;


#pragma region Attach interrupt methods

ISR(PCINT0_vect)
{
    clock.onInterrupt();
}

ISR(PCINT2_vect)
{
    clock.onInterrupt();
}

#pragma endregion


void setup()
{
    // LED strip setup
    FastLED.setMaxRefreshRate(24);
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    clock.setLeds(leds, NUM_LEDS);

    clock.begin();

    //Have a delay so the greeting message can be seen.
    delay(1000);
}


void loop()
{
    clock.mainloop();
}
