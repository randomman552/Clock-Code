#include "src/RTCHandler/RTCHandler.h"
#include "src/SevenSegHandler/SevenSegHandler.h"
#include "src/LEDStripController/LEDStripController.h"

//TODO: Implement EEPROMStore
//TODO: Implement BTNHandler
//TODO: Implement AlarmController

RTCHandler RTC;
SevenSegHandler SevenSeg(12, 11, 10, 8);

//LED Strip Setup
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 34
CRGB leds[NUM_LEDS];
LEDStripHandler LEDStrip;

//Buzzer Setup
#define BUZZER 13

void setup()
{
    //Initalise Serial
    Serial.begin(9600);
    Serial.println("Hello there!");
    SevenSeg.print("Helo");

    //Initalise LED Strip Controller
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    LEDStripHandler temp(leds, NUM_LEDS);
    LEDStrip = temp;

    //Initalise Buzzer
    pinMode(BUZZER, OUTPUT);

    //Have a delay so the greeting message can be seen.
    delay(1000);
}

void loop()
{
    DateTime time = RTC.getTime(true);
    Serial.println("Got time.");

    SevenSeg.displayTime(time, "{hour}.{min}");
    LEDStrip.LEDfx();

    //tone(BUZZER, 31, 1000);
}