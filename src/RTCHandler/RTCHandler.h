#ifndef RTCHandler_h
#define RTCHandler_h

//Include the required librarys
#include <Arduino.h>
#include <RTClib.h>

//Class for handling the RTC, automatically handles 12 hour time and daylight savings time.
class RTCHandler
{
private:
    RTC_DS3231 _rtc;

public:
    //Attributes
    int dayinMonth[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    //Constructors
    RTCHandler();

    //Functions
    void SetTime(DateTime time);

    //Get time functions
    DateTime getTime();
    DateTime getTime(bool twelveHour);

    //Format time functions
    DateTime FormatTime(DateTime time, bool twelveHour);

    //Useful boolean functions
    bool isAM();
    bool isDaylightSavings(DateTime time);
    bool isDaylightSavings();
};

#endif