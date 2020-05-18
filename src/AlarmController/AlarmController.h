#ifndef AlarmController_h
#define AlarmController_h

#include <Arduino.h>

/*
* Class for controlling the alarm.
* The checkAlarm method can be called with the current hour and minute, 
when it matches the internal alarm time the activePin given when instansiating this object will have a tone applied to it.
*/
class AlarmController
{
private:
    uint8_t _hour;
    uint8_t _minute;
    //The pin to activate when alarm triggers.
    uint8_t _pin;
    //Toggle boolean
    bool _active = true;

public:
    /*
    * Initalise the alarmController.
    * activePin (uint8_t) - The pin to activate when triggered.
    * alarmHour (uint8_t) - The hour at which the alarm should trigger.
    * alarmMinute (uint8_t) - The minute at which the alarm should trigger in that hour.
    */
    AlarmController(const uint8_t activePin, const uint8_t alarmHour, const uint8_t alarmMinute);

    /*
    * Initalise the alarm controller.
    * activePin (unsigned int) - The pin to activate when triggered.
    * Defaults to 7 AM.
    */
    AlarmController(const uint8_t activePin) : AlarmController(activePin, 7, 0){};

    /*
    * Initalise the alarm controller
    */
    AlarmController() : AlarmController(13) {}

    /*
    * Change the current alarm time.
    * alarmHour (unsigned int) - The hour at which the alarm should trigger.
    * alarmMinute (unsigned int) - The minute at which the alarm should trigger in that hour.
    */
    void setAlarmTime(const uint8_t alarmHour, const uint8_t alarmMinute);

    /*
    * Check if the alarm needs to go off and trigger the activePin if it does, this function will also return true.
    * curHour (unsigned int) - The hour to check against.
    * curMinute (unsigned int) - The minute to check against.
    */
    bool checkAlarm(const uint8_t curHour, const uint8_t curMinute);

    /*
    * This method is called when the alarm check passes. 
    * By default, it will play a tone on the activePin specified when instansiating this class.
    * But this can be overriden by creating a subclass of this alarmController class.
    */
    void activeAction();

    /*
    * Toggles whether the alarm is enabled.
    */
    void toggle();

    /*
    * Get the state of the alarm (true if enabled, false otherwise).
    */
    bool isEnabled();

    /*
    * Get the value of the alarm hour
    */
    int getAlarmHour();

    /*
    * Get the value of the alarm minute
    */
    int getAlarmMinute();
};

#endif