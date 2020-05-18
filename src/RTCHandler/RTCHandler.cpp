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
    if (isDaylightSavings(time))
    {
        //Take away 1 hours to undo our corrections for daylight savings time.
        time = time - TimeSpan(0, 1, 0, 0);
    }
    _rtc.adjust(time);
}

DateTime RTCHandler::FormatTime(DateTime time, const bool twelveHour)
{
    //Transform them according to the current month (daylight savings in Britain)
    if (isDaylightSavings(time))
    {
        //Add one hour
        time = time + TimeSpan(0, 1, 0, 0);
    }

    //If twelve hour is requested, take away 12 hours when the hour is greater than 12.
    if (twelveHour)
    {
        if (time.hour() > 12)
        {
            time = time - TimeSpan(0, 12, 0, 0);
        }
    }

    //Return our corrected DateTime object
    return time;
}

DateTime RTCHandler::getTime(const bool twelveHour)
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

bool RTCHandler::isDaylightSavings(const DateTime time)
{
    if (time.month() > 3 && time.month() < 10)
    {
        return true;
    }
    return false;
}
#pragma endregion