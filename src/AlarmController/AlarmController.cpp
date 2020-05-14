#include "AlarmController.h"

#pragma region constructors

AlarmController::AlarmController(unsigned const int activePin, unsigned const int gracePeriod, unsigned const int alarmHour, unsigned const int alarmMinute)
{
    _pin = activePin;
    _hour = alarmHour;
    _minute = alarmMinute;
}

#pragma endregion

#pragma region setters and getters

void AlarmController::setAlarmTime(unsigned const int alarmHour, unsigned const int alarmMinute)
{
    _hour = alarmHour;
    _minute = alarmMinute;
}

bool AlarmController::isEnabled()
{
    return _active;
}

#pragma endregion

#pragma region alarm functions

bool AlarmController::checkAlarm(unsigned const int curHour, unsigned const int curMinute)
{
    if (_active)
    {
        if (curHour == _hour)
        {
            if (curMinute >= _minute && curMinute <= _minute + 20)
            {
                activeAction();
            }
        }
    }
}

void AlarmController::activeAction()
{
    tone(_pin, 60, 1000);
}

void AlarmController::toggle()
{
    _active = !_active;
}

#pragma endregion