#include "src/RTCHandler/RTCHandler.h"
#include "src/SevenSegHandler/SevenSegHandler.h"
#include "src/LEDStripController/LEDStripController.h"
#include "src/EEPROMStore/EEPROMStore.h"

//TODO: Implement EEPROMStore
//TODO: Implement BTNHandler
//TODO: Implement AlarmController

RTCHandler RTC;
SevenSegHandler SevenSeg(12, 11, 10, 0);

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

    //Get values stored in EEPROM
    uint8_t effect = EEPROM.read(EFFECT_STORE);
    uint8_t red = EEPROM.read(RED_STORE);
    uint8_t green = EEPROM.read(GREEN_STORE);
    uint8_t blue = EEPROM.read(BLUE_STORE);
    uint8_t brightness = EEPROM.read(BRIGHTNESS_STORE);

    //Apply brightness to Seven Seg
    SevenSeg.setBrightness(brightness);

    //Initalise LED Strip Controller
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    LEDStripHandler temp(leds, NUM_LEDS, brightness, effect, 1000, CRGB(red, green, blue));
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