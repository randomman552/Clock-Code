
#ifndef SevenSegHandler_h
#define SevenSegHandler_h

#include <Arduino.h>
#include <LedControl.h>
#include <RTClib.h>

//TODO: Should try to make this take up less RAM (move to progmem)
const String dayofWeek[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
const String monthofYear[] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};

//This class uses the LEDControl library to display data on a seven segment display.
class SevenSegHandler
{
private:
    int _brightness;
    int _deviceNum;
    LedControl _lc;

    //Functions
    String _digitFormatter(String toFormat);

public:
    //Constructors
    SevenSegHandler(int data_pin, int clk_pin, int load_pin, int max_num, int brightness);

    //Functions
    void
    setBrightness(int newBrightness);
    void print(String toPrint);
    void displayTime(DateTime time, String format);
};

#endif