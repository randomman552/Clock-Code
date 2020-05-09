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
    /*
    * RTCHandler Constructor
    * Takes no arguments
    */
    RTCHandler();

    //Functions
    /*
    * Sets time on the RTC to the given time (time must be 24 hour format).
    * time (DateTime) - The time to set to.
    */
    void SetTime(DateTime time);

    //Get time functions
    /*
    * Function to get current time, accounts for daylight savings time.
    * return (DateTime) - The current time.
    */
    DateTime getTime();
    /*
    * Function to get current time, accounts for daylight savings time and 12 hour mode.
    * twelveHour (bool) - Whether the time should be converted to twelve hour time or not.
    * return (DateTime) - The current time.
    */
    DateTime getTime(bool twelveHour);

    /*
    * Format the given time according to daylight savings and 12 hour mode.
    * time (DateTime) - The time to format.
    * twelveHour (bool) - Whether the time should be converted to twelve hour time or not.
    * return (DateTime) - The updated DateTime object.
    */
    DateTime FormatTime(DateTime time, bool twelveHour);

    //Useful boolean functions
    /*
    * Check whether current time is AM or PM.
    * return (bool) - True if currently AM.
    */
    bool isAM();

    /*
    * Overload of isDaylightSavings(time).
    * Returns true if the time is currently in daylight savings otherwise returns false.
    * return (bool) - True if in daylight savings time.
    */
    bool isDaylightSavings(DateTime time);

    /*
    * Returns true if the time is currently in daylight savings otherwise returns false.
    * param time - The time to check
    * return (bool) - True if in daylight savings time.
    */
    bool isDaylightSavings();
};

#endif