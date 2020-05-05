#include <Arduino.h>
#include "RTCHandler.h"

//RTCHandler Constructor
RTCHandler::RTCHandler()
{
    _rtc.begin();
}

/*
* Sets time on the RTC to the given time (time must be 24 hour format). 
* Accounts for british daylight savings time.
* time (DateTime) - The time to set to.
*/
void RTCHandler::SetTime(DateTime time)
{
    _rtc.adjust(time);
}

/*
* Format the given time according to daylight savings and 12 hour mode.
* time (DateTime) - The time to format.
* twelveHour (bool) - Whether the time should be converted to twelve hour time or not.
* return (DateTime) - The updated DateTime object.
*/
DateTime RTCHandler::FormatTime(DateTime time, bool twelveHour)
{
    //Get all the variables to handle wrap around for
    int hour = time.hour();
    int day = time.day();
    int month = time.month();
    int year = time.year();

    //Transform them according to the current month (daylight savings in Britain)
    if (isDaylightSavings(time))
    {
        //Add 1 to the hour
        hour += 1;

        //If the hour is greater than 24, wrap it around
        // And add 1 to the day
        if (hour > 24)
        {
            hour = hour % 24;
            day++;

            //If we have increase the hour, we must now check the day is correct
            //If the day is greater than the length of the current month, wrap it around
            //And increase the month by 1
            if (day > dayinMonth[month])
            {
                //We modulus by the number of days in the month -1 so that it starts at 1 instead of 0.
                day = day % (dayinMonth[month] - 1);
                month++;

                //If we have increased the month, we must now check the year
                //If the month is greater than 12, then wrap it around
                //And increase the year by 1
                if (month > 12)
                {
                    month = month % 11;
                    year++;
                }
            }
        }
    }

    //Transform them according to the twelveHour boolean
    if (twelveHour)
    {
        //Modulo the hour by 12, this will set it to the right value
        hour = hour % 12;
    }

    //Return our corrected DateTime object
    return DateTime(year, month, day, hour, time.minute(), time.second());
}

/*
* Function to get current time, accounts for daylight savings time and 12 hour mode.
* twelveHour (bool) - Whether the time should be converted to twelve hour time or not.
* return (DateTime) - The current time.
*/
DateTime RTCHandler::getTime(bool twelveHour)
{
    //Get the current date and time in a DateTime object
    DateTime now = _rtc.now();

    //Format the time
    now = FormatTime(now, twelveHour);

    return DateTime();
}

/*
* Function to get current time, accounts for daylight savings time.
* return (DateTime) - The current time.
*/
DateTime RTCHandler::getTime()
{
    return getTime(false);
}

/*
* Check whether current time is AM or PM.
* return (bool) - True if currently AM.
*/
bool RTCHandler::isAM()
{
    DateTime time = getTime();
    if (time.hour() > 12)
    {
        return false;
    }
    return true;
}

/*
* Overload of isDaylightSavings(time).
* Returns true if the time is currently in daylight savings otherwise returns false.
* return (bool) - True if in daylight savings time.
*/
bool RTCHandler::isDaylightSavings()
{
    return isDaylightSavings(getTime());
}

/*
* Returns true if the time is currently in daylight savings otherwise returns false.
* param time - The time to check
* return (bool) - True if in daylight savings time.
*/
bool RTCHandler::isDaylightSavings(DateTime time)
{
    if (time.month() > 3 && time.month() < 10)
    {
        return true;
    }
    return false;
}