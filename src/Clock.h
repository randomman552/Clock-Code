#ifndef CLOCK_h
#define CLOCK_h

#include <Arduino.h>
#include <RTClib.h>
#include <LEDStripController.h>

#include "SevenSeg.h"
#include "util.h"

/**
 * @brief Clock class which handles all functionality of the clock
 * NOTE: Clock.begin() must be called in the setup method for this to work.
 * Clock.mainloop() should then be called in the main arduino loop.
 */
class Clock
{
    private:
        RTC_DS3231 _rtc;
        LEDStripController::Controller _ledStrip;
        SevenSeg _sevenSeg;

        bool _12Hour = true;
        DateTime _time;
        /**
         * @brief Pin that feedback should be provided to.
         * It is assumed that this is a buzzer.
         */
        uint8_t _feedbackPin = 13;
        
        // Pins to query as buttons
        const uint8_t _btnPins[4] = { 8, 7, 6, 5 };

        // States of all buttons
        volatile bool _btns[4] = { false };

        void enableInterrupts() { PCICR = B00000101; };
        void disableInterrupts() { PCICR = B00000000; };
        
        /**
         * @brief Method called to resolve button states after the pin change interrupt has been called.
         */
        void onPressed();

        /**
         * @brief Reset button states to all off.
         */
        void clearButtons();

        /**
         * @brief Utility method for checking if any buttons are pressed.
         */
        bool buttonPressed();

        /**
         * @brief Method used to play the feedback tone to the feedback pin.
         */
        void playFeedback();

        /**
         * @brief Method allowing the user to control the internal values of the clock object
         * Interrupts control flow into the user provides a new value or cancels the action.
         * Uses the same four buttons as the clock does normally.
         * @param startVal The intial value to display
         * @param min The minimum value to allow (inclusive)
         * @param max The maximum value to allow (exclusive)
         * @return The value after changing
         */
        int changeValue(const int startVal, int min, int max);
        
        /**
         * @brief Method used to display time as part of "mainloop"
         */
        void displayTime();
    public:

        Clock();
        ~Clock();

        /**
         * @brief Begin method must be called in setup function.
         */
        void begin();
        void mainloop();

        /**
         * Set the LEDStripController objects led array.
         */
        void setLeds(CRGB* leds, int numLEDs) { _ledStrip.setLEDs(leds, numLEDs); };
        
        /**
         * @brief Method to handle call from ISR defined in main .ino file.
         */
        void onInterrupt();
};


#endif