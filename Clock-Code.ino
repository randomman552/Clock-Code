#include "src/millisDelay.h"
#include "src/RTCHandler/RTCHandler.h"
#include "src/SevenSegHandler/SevenSegHandler.h"
#include "src/LEDStripController/LEDStripController.h"
#include "src/EEPROMStore/EEPROMStore.h"
#include "src/AlarmController/AlarmController.h"
#include "src/BTNHandler/BTNHandler.h"

//Buzzer Setup
#define BUZZER 13

//Create our required objects
RTCHandler RTC;
SevenSegHandler SevenSeg(12, 11, 10, 0);
AlarmController Alarm(BUZZER, 5);
LEDStripHandler LEDStrip;
BTNHandler BTNS(RTC, SevenSeg, Alarm, LEDStrip, BUZZER);

//LED Strip Setup
#define DATA_PIN 9
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define NUM_LEDS 34
CRGB leds[NUM_LEDS];

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

    //Get values stored in EEPROM
    uint8_t effect = EEPROM.read(EFFECT_STORE);
    uint8_t red = EEPROM.read(RED_STORE);
    uint8_t green = EEPROM.read(GREEN_STORE);
    uint8_t blue = EEPROM.read(BLUE_STORE);
    uint8_t brightness = EEPROM.read(BRIGHTNESS_STORE);
    uint8_t alarmHour = EEPROM.read(ALARM_HOUR_STORE);
    uint8_t alarmMinute = EEPROM.read(ALARM_MINUTE_STORE);

    //Apply saved settings
    SevenSeg.setBrightness(brightness);
    LEDStrip.setBrightness(brightness);
    LEDStrip.setEffect(effect);
    LEDStrip.setDelay(2000);
    LEDStrip.setRGB(red, green, blue);
    Alarm.setAlarmTime(alarmHour, alarmMinute);

    //Have a delay so the greeting message can be seen.
    millisDelay(1000);
}

//Attach the interupt method
ISR(PCINT0_vect)
{
    BTNS.interruptAction();
}

ISR(PCINT2_vect)
{
    BTNS.interruptAction();
}

void loop()
{
    for (int i = 1; i <= 100; i++)
    {
        DateTime time24h = RTC.getTime();
        DateTime time12h = RTC.FormatTime(time24h, true);

        //Every 10th loop, we display the month and day
        if (i % 10 == 0)
        {
            SevenSeg.displayTime(time12h, "{day}:{month}");
        }
        //On the next loop, display the year
        else if ((i % 10) - 1 == 0)
        {
            SevenSeg.displayTime(time12h, "20{year}.");
        }
        else
        {
            //Display time normally
            SevenSeg.displayTime(time12h, "{hour}:{min}");
        }

        //Show the LEDEffect
        LEDStrip.LEDfx();

        //Check the alarm.
        Alarm.checkAlarm(time24h.hour(), time24h.minute());
        BTNS.btnAction();
    }

    //Only update the EEPROM every 100 loops.
    updateEEPROMStore(LEDStrip.getEffect(), LEDStrip.color, LEDStrip.getBrightness());
    updateAlarmTime(Alarm.getAlarmHour(), Alarm.getAlarmMinute());
}