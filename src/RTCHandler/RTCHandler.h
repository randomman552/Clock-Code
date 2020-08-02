#ifndef RTCHandler_h
#define RTCHandler_h

//Include the required librarys
#include <Arduino.h>
#include <RTClib.h>

#pragma region PROGMEM month

//Store months of year in PROGMEM
const char monthofYear1[] PROGMEM = "january";
const char monthofYear2[] PROGMEM = "february";
const char monthofYear3[] PROGMEM = "march";
const char monthofYear4[] PROGMEM = "april";
const char monthofYear5[] PROGMEM = "may";
const char monthofYear6[] PROGMEM = "june";
const char monthofYear7[] PROGMEM = "july";
const char monthofYear8[] PROGMEM = "august";
const char monthofYear9[] PROGMEM = "september";
const char monthofYear10[] PROGMEM = "october";
const char monthofYear11[] PROGMEM = "november";
const char monthofYear12[] PROGMEM = "december";

//Create table to refer to the month strings
const char *const monthofYear[] PROGMEM = {monthofYear1, monthofYear2, monthofYear3, monthofYear4,
                                           monthofYear5, monthofYear6, monthofYear7, monthofYear8,
                                           monthofYear9, monthofYear10, monthofYear11, monthofYear12};

#pragma endregion

#pragma region PROGMEM days of week
//Store days of week in PROGMEM
const char dayofWeek1[] PROGMEM = "sunday";
const char dayofWeek2[] PROGMEM = "monday";
const char dayofWeek3[] PROGMEM = "tuesday";
const char dayofWeek4[] PROGMEM = "wednesday";
const char dayofWeek5[] PROGMEM = "thursday";
const char dayofWeek6[] PROGMEM = "friday";
const char dayofWeek7[] PROGMEM = "saturday";

//Create table to refer to the day strings
const char *const dayofWeek[] PROGMEM = {dayofWeek1, dayofWeek2, dayofWeek3, dayofWeek5,
                                         dayofWeek6, dayofWeek7};

#pragma endregion

//Class for handling the RTC, automatically handles 12 hour time and daylight savings time.
class RTCHandler
{
private:
    RTC_DS3231 _rtc;

public:
    //Attributes
    const int dayinMonth[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
    DateTime getTime(const bool twelveHour);

    /*
    * Format the given time according to daylight savings and 12 hour mode.
    * time (DateTime) - The time to format.
    * twelveHour (bool) - Whether the time should be converted to twelve hour time or not.
    * return (DateTime) - The updated DateTime object.
    */
    DateTime FormatTime(DateTime time, const bool twelveHour);

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
    bool isDaylightSavings(const DateTime time);

    /*
    * Returns true if the time is currently in daylight savings otherwise returns false.
    * param time - The time to check
    * return (bool) - True if in daylight savings time.
    */
    bool isDaylightSavings();
};

#endif