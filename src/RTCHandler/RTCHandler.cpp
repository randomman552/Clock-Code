#include <Arduino.h>
#include "RTCHandler.h"

#pragma region Constructors

RTCHandler::RTCHandler()
{
    _rtc.begin();
}

#pragma endregion

#pragma region DateTime related functions

void RTCHandler::SetTime(DateTime time)
{
    _rtc.adjust(time);
}

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

DateTime RTCHandler::getTime(bool twelveHour)
{
    //Get the current date and time in a DateTime object
    DateTime now = _rtc.now();

    //Format the time
    now = FormatTime(now, twelveHour);

    return now;
}

DateTime RTCHandler::getTime()
{
    return getTime(false);
}

bool RTCHandler::isAM()
{
    DateTime time = getTime();
    if (time.hour() > 12)
    {
        return false;
    }
    return true;
}

bool RTCHandler::isDaylightSavings()
{
    return isDaylightSavings(getTime());
}

bool RTCHandler::isDaylightSavings(DateTime time)
{
    if (time.month() > 3 && time.month() < 10)
    {
        return true;
    }
    return false;
}
#pragma endregion