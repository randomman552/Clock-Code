#include "BTNHandler.h"

BTNHandler::BTNHandler(RTCHandler &RTC, SevenSegHandler &SevenSeg, AlarmController &Alarm, LEDStripHandler &LEDStrip, const int feedbackPin)
    : _RTC(RTC),
      _SevenSeg(SevenSeg),
      _Alarm(Alarm),
      _LEDStrip(LEDStrip)

{
    //Set pin modes
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(BTN3, INPUT);
    pinMode(BTN4, INPUT);

    //Clear the PCINT flag register
    PCIFR = B00000000;

    //Enable the interupt for pins D0 - D7 and D8 - D13 (for the respective bits)
    PCICR = B00000101;

    //Set the pin masks so that only pin 5-8 are used for interupting.
    PCMSK0 = B00000001;
    PCMSK2 = B11100000;
}

void BTNHandler::enableInterrupts()
{
    PCICR = B00000101;
}

void BTNHandler::disableInterrupts()
{
    PCICR = B00000000;
}

void BTNHandler::btnAction()
{
    //Disable interrupts
    disableInterrupts();

    //Store the values from the btnStates variable for easy access
    bool btn1 = _btnStates[0];
    bool btn2 = _btnStates[1];
    bool btn3 = _btnStates[2];
    bool btn4 = _btnStates[3];

    //If any of the buttons are pressed
    if (btn1 || btn2 || btn3 || btn4)
    {
        if (btn1 && btn2)
        {
            /* Change alarm */
            int newHour = changeInt(_Alarm.getAlarmHour(), 0, 23);
            int newMinute = changeInt(_Alarm.getAlarmMinute(), 0, 59);
            _Alarm.setAlarmTime(newHour, newMinute);
        }
        else if (btn3 && btn4)
        {
            /* Change time */
            DateTime curTime = _RTC.getTime();
            int newYear = changeInt(curTime.year(), 2000, 2099);
            int newMonth = changeInt(curTime.month(), 1, 12);
            int newDay = changeInt(curTime.day(), 1, _RTC.dayinMonth[newMonth - 1]);
            //Add one to the intial hour value, as the RTC stores it starting from 0.
            int newHour = changeInt(curTime.hour() + 1, 1, 24);
            //Take away the extra added hour
            newHour--;
            int newMinute = changeInt(curTime.minute(), 0, 59);
            DateTime newTime(newYear, newMonth, newDay, newHour, newMinute);
            _RTC.SetTime(newTime);
        }
        else if (btn1 && btn3)
        {
            /* Change custom color */
            CRGB curColor = _LEDStrip.color;
            int newRed = changeInt(curColor.red, 0, 255);
            int newGreen = changeInt(curColor.green, 0, 255);
            int newBlue = changeInt(curColor.blue, 0, 255);
            int newBrightness = changeInt(_LEDStrip.getBrightness(), 0, 100);
            _LEDStrip.setRGB(newRed, newGreen, newBlue);
            _LEDStrip.setBrightness(newBrightness);
            _SevenSeg.setBrightness(newBrightness);
        }
        else if (btn2 && btn4)
        {
            /* EMPTY BIND */
        }
        else if (btn1)
        {
            /* Toggle low power mode */
            uint8_t brightness = EEPROM.read(BRIGHTNESS_STORE);
            //If the clock is already in low power mode disable it, otherwise enable it.
            if (_lowPowerMode)
            {
                _SevenSeg.setBrightness(brightness);
            }
            else
            {
                //Set seven segment display brightness to 50% of what it is normally.
                _SevenSeg.setBrightness(brightness / 2);
            }
            //Toggle LEDStrip.
            _LEDStrip.toggle();

            //Flip lowPowerMode state.
            _lowPowerMode = !_lowPowerMode;
        }
        else if (btn2)
        {
            /* Toggle the alarm */
            _Alarm.toggle();
        }
        else if (btn3)
        {
            /*Increase the effect on the LED strip by 1, 
            then modulus by the max effect + 1 to wrap it around to 0 if above the max value.*/
            _LEDStrip.setEffect((_LEDStrip.getEffect() + 1) % (_LEDStrip.getMaxEffect() + 1));
        }
        else if (btn4)
        {
            /*Decrease the effect on the LED strip by 1, if it is below zero, set to the max effect.
            The C implementation doesn't wrap negative numbers around like it might in other languages.*/
            int effect = _LEDStrip.getEffect();
            effect--;
            if (effect < 0)
            {
                effect = _LEDStrip.getMaxEffect();
            }

            //Apply new effect value
            _LEDStrip.setEffect(effect);
        }
    }

    //Reset the btnStates
    for (unsigned int i = 0; i < 4; i++)
    {
        _btnStates[i] = false;
    }

    //Re-enable the interrupts.
    enableInterrupts();
}

void BTNHandler::interruptAction()
{
    //Clear flag register
    PCIFR = B00000000;

    //Give the user a grace period, as they may not press two buttons at EXACTLY the same time.
    for (unsigned int i = 0; i < 50; i++)
    {
        if (digitalRead(BTN1) == HIGH)
        {
            _btnStates[0] = true;
        }
        if (digitalRead(BTN2) == HIGH)
        {
            _btnStates[1] = true;
        }
        if (digitalRead(BTN3) == HIGH)
        {
            _btnStates[2] = true;
        }
        if (digitalRead(BTN4) == HIGH)
        {
            _btnStates[3] = true;
        }
        //Play feedback
        digitalWrite(_feedbackPin, HIGH);
        delayMicroseconds(1000);
        digitalWrite(_feedbackPin, LOW);
        delayMicroseconds(1000);
    }

    //Display the userfeedback to the display now, to make it look more responsive.
    if (_btnStates[0] && _btnStates[1])
    {
        _SevenSeg.print("a-ch");
    }
    else if (_btnStates[2] && _btnStates[3])
    {
        _SevenSeg.print("t-ch");
    }
    else if (_btnStates[0] && _btnStates[2])
    {
        _SevenSeg.print("c-ch");
    }
    else if (_btnStates[1] && _btnStates[3])
    {
        /* EMPTY BIND */
    }
    else if (_btnStates[0])
    {
        if (_lowPowerMode)
        {
            _SevenSeg.print("l-on");
        }
        else
        {
            _SevenSeg.print("loff");
        }
    }
    else if (_btnStates[1])
    {
        if (_Alarm.isEnabled())
        {
            _SevenSeg.print("aoff");
        }
        else
        {
            _SevenSeg.print("a-on");
        }
    }
    else if (_btnStates[2])
    {
        _SevenSeg.print("led+");
    }
    else if (_btnStates[3])
    {
        _SevenSeg.print("led-");
    }

    //Block until all buttons are released
    while (digitalRead(BTN1) || digitalRead(BTN2) || digitalRead(BTN3) || digitalRead(BTN4))
    {
    }
}

int BTNHandler::changeInt(int startValue, int min, int max)
{
    int newValue = startValue;
    while (true)
    {
        //Convert value to String, and print it to the Display
        char strBuffer[5] = {0};
        itoa(newValue, strBuffer, 10);
        //Serial.println(strBuffer);
        _SevenSeg.print(strBuffer, true);
        bool btns[4] = {digitalRead(BTN1), digitalRead(BTN2), digitalRead(BTN3), digitalRead(BTN4)};

        //If any of the buttons are pressed, play user feedback from the buzzer.
        if (btns[0] || btns[1] || btns[0] || btns[1])
        {
            tone(_feedbackPin, 60, 100);
        }

        if (btns[0])
        {
            //Wait for the user to release the button
            while (digitalRead(BTN1))
            {
            }

            //Return the new value
            return newValue;
        }
        else if (btns[1])
        {
            //Wait for the user to release the button
            while (digitalRead(BTN2))
            {
            }

            //Cancel the change, and return the starting value.
            return startValue;
        }
        else if (btns[2])
        {
            //Increment the value, and make sure it falls within the allowed values.
            newValue++;
            if (newValue > max)
            {
                newValue = min;
            }
        }
        else if (btns[3])
        {
            //Decrement the value, and make sure it falls within the allowed values.
            newValue--;
            if (newValue < min)
            {
                newValue = max;
            }
        }
        //Have a delay in the loop to limit the rate of change.
        millisDelay(100);
    }
}
