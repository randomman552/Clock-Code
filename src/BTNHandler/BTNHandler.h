//TODO: Move this all into a CPP file and make a class.
//This is currently very messy.

#ifndef BTNHandler_h
#define BTNHandler_h

#include <Arduino.h>
#include "../millisDelay.h"
#include "../RTCHandler/RTCHandler.h"
#include "../SevenSegHandler/SevenSegHandler.h"
#include "../LEDStripController/LEDStripController.h"
#include "../AlarmController/AlarmController.h"
#include "../EEPROMStore/EEPROMStore.h"

//This tutorial was very helpful for the interupts
//https://www.teachmemicro.com/arduino-interrupt-tutorial/

//Define button pins
#define BTN1 8
#define BTN2 7
#define BTN3 6
#define BTN4 5

class BTNHandler
{
private:
    RTCHandler &_RTC;
    SevenSegHandler &_SevenSeg;
    AlarmController &_Alarm;
    LEDStripHandler &_LEDStrip;
    int _feedbackPin;
    volatile bool _btnStates[4] = {false};
    volatile bool _lowPowerMode = false;

public:
    BTNHandler(RTCHandler &RTC, SevenSegHandler &SevenSeg, AlarmController &Alarm, LEDStripHandler &LEDStrip, const int feedbackPin);

    /*
    * This function takes the button states set by the interuptAction interupt, and then acts on them accordingly.
    * It should be called in the main event loop.
    * This is done in the main event loop to prevent the interupt function from becoming really long.
    */
    void btnAction();

    /*
    * This sets the global button states, which are then acted on in the btnAction function.
    * This disables interrupts until the btnAction function is called.
    */
    void interruptAction();

    /*
    * Method which uses the buttons and seven segment display on the clock to 
    enable the changing of an int.
    * This method uses BTN3 and BTN4 to change values, and BTN1 and BTN2 for confirm and cancel respectively.
    * startValue (int) - The inital value to start with.
    * min (int) - The minimum value which can be set to.
    * max (int) - The maximum value it can be set to.
    * return (int) - The new value.
    */
    int changeInt(const int startValue, int min, int max);

    //Disable pin change interrupts
    void disableInterrupts();

    //Enable pin change interrupts
    void enableInterrupts();
};

#endif