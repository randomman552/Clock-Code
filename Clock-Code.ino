#include "src/RTCHandler/RTCHandler.h"
#include "src/SevenSegHandler/SevenSegHandler.h"
#include "src/LEDStripController/LEDStripController.h"
#include "src/EEPROMStore/EEPROMStore.h"
#include "src/AlarmController/AlarmController.h"

//Buzzer Setup
#define BUZZER 13

//Create our required objects
RTCHandler RTC;
SevenSegHandler SevenSeg(12, 11, 10, 0);
AlarmController Alarm(BUZZER, 5);
LEDStripHandler LEDStrip;

//LED Strip Setup
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 34
CRGB leds[NUM_LEDS];

//Button handler is imported here so that it can use the variables from this file.
#include "src/BTNHandler/BTNHandler.h"

void setup()
{
    //Initalise Serial
    Serial.begin(9600);
    Serial.println("Hello there!");
    SevenSeg.print("Helo");

    //Initalise LED Strip Controller
    FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    LEDStrip.init(leds, NUM_LEDS);

    //Initalise Buzzer
    pinMode(BUZZER, OUTPUT);

    //Initalise PCINT
    PCINT_setup();

    //Get values stored in EEPROM
    uint8_t effect = EEPROM.read(EFFECT_STORE);
    uint8_t red = EEPROM.read(RED_STORE);
    uint8_t green = EEPROM.read(GREEN_STORE);
    uint8_t blue = EEPROM.read(BLUE_STORE);
    uint8_t brightness = EEPROM.read(BRIGHTNESS_STORE);

    //Apply saved settings
    SevenSeg.setBrightness(brightness);
    LEDStrip.setBrightness(brightness);
    LEDStrip.setEffect(effect);
    LEDStrip.setDelay(2000);
    LEDStrip.setRGB(red, green, blue);
    Alarm.setAlarmTime(2, 30);

    //Have a delay so the greeting message can be seen.
    delay(1000);
}

void loop()
{
    DateTime time = RTC.getTime(true);

    SevenSeg.displayTime(time, "{hour}.{min}");
    LEDStrip.LEDfx();

    updateEEPROMStore(LEDStrip.getEffect(), LEDStrip.color, LEDStrip.getBrightness());

    Alarm.checkAlarm(time.hour(), time.minute());
}