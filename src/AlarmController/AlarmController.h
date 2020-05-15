#ifndef AlarmController_h
#define AlarmContoller_h

#include <Arduino.h>

/*
* Class for controlling the alarm.
* The checkAlarm method can be called with the current hour and minute, 
when it matches the internal alarm time the activePin given when instansiating this object will have a tone applied to it.
*/
class AlarmController
{
private:
    unsigned int _hour;
    unsigned int _minute;
    //The last minute the alarm was triggered.
    unsigned int _lastActivation;
    //The pin to activate when alarm triggers.
    unsigned int _pin;
    //Toggle boolean
    bool _active = true;

public:
    /*
    * Initalise the alarmController.
    * activePin (unsigned int) - The pin to activate when triggered.
    * alarmHour (unsigned int) - The hour at which the alarm should trigger.
    * alarmMinute (unsigned int) - The minute at which the alarm should trigger in that hour.
    */
    AlarmController(unsigned const int activePin, unsigned const int gracePeriod, unsigned const int alarmHour, unsigned const int alarmMinute);

    /*
    * Initalise the alarmController.
    * activePin (unsigned int) - The pin to activate when triggered.
    * Defaults to 7 AM.
    */
    AlarmController(unsigned const int activePin, unsigned const int gracePeriod) : AlarmController(activePin, gracePeriod, 7, 0){};

    /*
    * Change the current alarm time.
    * alarmHour (unsigned int) - The hour at which the alarm should trigger.
    * alarmMinute (unsigned int) - The minute at which the alarm should trigger in that hour.
    */
    void setAlarmTime(unsigned const int alarmHour, unsigned const int alarmMinute);

    /*
    * Check if the alarm needs to go off and trigger the activePin if it does, this function will also return true.
    * curHour (unsigned int) - The hour to check against.
    * curMinute (unsigned int) - The minute to check against.
    */
    bool checkAlarm(unsigned int const curHour, unsigned int const curMinute);

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