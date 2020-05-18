#include "AlarmController.h"

#pragma region constructors

AlarmController::AlarmController(const uint8_t activePin, const uint8_t alarmHour, const uint8_t alarmMinute)
{
    _pin = activePin;
    _hour = alarmHour;
    _minute = alarmMinute;
}

#pragma endregion

#pragma region setters and getters

void AlarmController::setAlarmTime(const uint8_t alarmHour, const uint8_t alarmMinute)
{
    _hour = alarmHour;
    _minute = alarmMinute;
}

bool AlarmController::isEnabled()
{
    return _active;
}

int AlarmController::getAlarmHour()
{
    return _hour;
}

int AlarmController::getAlarmMinute()
{
    return _minute;
}

#pragma endregion

#pragma region alarm functions

bool AlarmController::checkAlarm(const uint8_t curHour, const uint8_t curMinute)
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