//TODO: Move this all into a CPP file and make a class.
//This is currently very messy.

#ifndef BTNHandler_h
#define BTNHandler_h

#include <Arduino.h>
#include "../millisDelay.h"

//This tutorial was very helpful for the interupts
//https://www.teachmemicro.com/arduino-interrupt-tutorial/

//Define button pins
#define BTN1 8
#define BTN2 7
#define BTN3 6
#define BTN4 5

volatile bool lowPowerMode = false;
volatile bool btnStates[4] = {false};

#pragma region setup functions

void PCINT_setup()
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

//Disable pin change interrupts
void disableInterrupts()
{
    PCICR = B00000000;
}

//enable pin change interrupts
void enableInterrupts()
{
    PCICR = B00000101;
}

#pragma endregion

#pragma region value change functions

/*
* Function which uses the buttons and seven segment display on the clock to 
enable the changing of an int.
* This function uses BTN3 and BTN4 to change values, and BTN1 and BTN2 for confirm and cancel respectively.
* startValue (int) - The inital value to start with.
* min (int) - The minimum value which can be set to.
* max (int) - The maximum value it can be set to.
* return (int) - The new value.
*/
int changeInt(int startValue, int min, int max)
{
    int newValue = startValue;
    while (true)
    {
        //Convert value to String, and print it to the Display
        char strBuffer[5] = {0};
        itoa(newValue, strBuffer, 10);
        //Serial.println(strBuffer);
        SevenSeg.print(strBuffer, true);
        bool btns[4] = {digitalRead(BTN1), digitalRead(BTN2), digitalRead(BTN3), digitalRead(BTN4)};

        //If any of the buttons are pressed, play user feedback from the buzzer.
        if (btns[0] || btns[1] || btns[0] || btns[1])
        {
            tone(BUZZER, 60, 100);
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

#pragma endregion

#pragma region action functions
/*
* This sets the global button states, which are then acted on in the btnAction function.
* This disables interrupts until the btnAction function is called.
*/
void interruptAction()
{
    //Clear flag register
    PCIFR = B00000000;

    //Give the user a grace period, as they may not press two buttons at EXACTLY the same time.
    for (unsigned int i = 0; i < 50; i++)
    {
        if (digitalRead(BTN1) == HIGH)
        {
            btnStates[0] = true;
        }
        if (digitalRead(BTN2) == HIGH)
        {
            btnStates[1] = true;
        }
        if (digitalRead(BTN3) == HIGH)
        {
            btnStates[2] = true;
        }
        if (digitalRead(BTN4) == HIGH)
        {
            btnStates[3] = true;
        }
        //Play feedback
        digitalWrite(BUZZER, HIGH);
        delayMicroseconds(1000);
        digitalWrite(BUZZER, LOW);
        delayMicroseconds(1000);
    }

    //Display the userfeedback to the display now, to make it look more responsive.
    if (btnStates[0] && btnStates[1])
    {
        SevenSeg.print("a-ch");
    }
    else if (btnStates[2] && btnStates[3])
    {
        SevenSeg.print("t-ch");
    }
    else if (btnStates[0] && btnStates[2])
    {
        SevenSeg.print("c-ch");
    }
    else if (btnStates[1] && btnStates[3])
    {
        /* EMPTY BIND */
    }
    else if (btnStates[0])
    {
        if (lowPowerMode)
        {
            SevenSeg.print("l-on");
        }
        else
        {
            SevenSeg.print("loff");
        }
    }
    else if (btnStates[1])
    {
        if (Alarm.isEnabled())
        {
            SevenSeg.print("aoff");
        }
        else
        {
            SevenSeg.print("a-on");
        }
    }
    else if (btnStates[2])
    {
        SevenSeg.print("led+");
    }
    else if (btnStates[3])
    {
        SevenSeg.print("led-");
    }

    //Block until all buttons are released
    while (digitalRead(BTN1) || digitalRead(BTN2) || digitalRead(BTN3) || digitalRead(BTN4))
    {
    }
}

/*
* This function takes the button states set by the interuptAction interupt, and then acts on them accordingly.
* It should be called in the main event loop.
* This is done in the main event loop to prevent the interupt function from becoming really long.
*/
void btnAction()
{
    //Disable interrupts
    disableInterrupts();

    //Store the values from the btnStates variable for easy access
    bool btn1 = btnStates[0];
    bool btn2 = btnStates[1];
    bool btn3 = btnStates[2];
    bool btn4 = btnStates[3];

    //If any of the buttons are pressed
    if (btn1 || btn2 || btn3 || btn4)
    {
        if (btn1 && btn2)
        {
            /* Change alarm */
            int newHour = changeInt(Alarm.getAlarmHour(), 0, 23);
            int newMinute = changeInt(Alarm.getAlarmMinute(), 0, 59);
            Alarm.setAlarmTime(newHour, newMinute);
        }
        else if (btn3 && btn4)
        {
            /* Change time */
            DateTime curTime = RTC.getTime();
            int newYear = changeInt(curTime.year(), 2000, 2099);
            int newMonth = changeInt(curTime.month(), 1, 12);
            int newDay = changeInt(curTime.day(), 1, RTC.dayinMonth[newMonth - 1]);
            //Add one to the intial hour value, as the RTC stores it starting from 0.
            int newHour = changeInt(curTime.hour() + 1, 1, 24);
            //Take away the extra added hour
            newHour--;
            int newMinute = changeInt(curTime.minute(), 0, 59);
            DateTime newTime(newYear, newMonth, newDay, newHour, newMinute);
            RTC.SetTime(newTime);
        }
        else if (btn1 && btn3)
        {
            /* Change custom color */
            CRGB curColor = LEDStrip.color;
            int newRed = changeInt(curColor.red, 0, 255);
            int newGreen = changeInt(curColor.green, 0, 255);
            int newBlue = changeInt(curColor.blue, 0, 255);
            int newBrightness = changeInt(LEDStrip.getBrightness(), 0, 100);
            LEDStrip.setRGB(newRed, newGreen, newBlue);
            LEDStrip.setBrightness(newBrightness);
            SevenSeg.setBrightness(newBrightness);
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
            if (lowPowerMode)
            {
                SevenSeg.setBrightness(brightness);
            }
            else
            {
                //Set seven segment display brightness to 50% of what it is normally.
                SevenSeg.setBrightness(brightness / 2);
            }
            //Toggle LEDStrip.
            LEDStrip.toggle();

            //Flip lowPowerMode state.
            lowPowerMode = !lowPowerMode;
        }
        else if (btn2)
        {
            /* Toggle the alarm */
            Alarm.toggle();
        }
        else if (btn3)
        {
            /*Increase the effect on the LED strip by 1, 
            then modulus by the max effect + 1 to wrap it around to 0 if above the max value.*/
            LEDStrip.setEffect((LEDStrip.getEffect() + 1) % (LEDStrip.getMaxEffect() + 1));
        }
        else if (btn4)
        {
            /*Decrease the effect on the LED strip by 1, if it is below zero, set to the max effect.
            The C implementation doesn't wrap negative numbers around like it might in other languages.*/
            int effect = LEDStrip.getEffect();
            effect--;
            if (effect < 0)
            {
                effect = LEDStrip.getMaxEffect();
            }

            //Apply new effect value
            LEDStrip.setEffect(effect);
        }

        delay(1000);
    }

    //Reset the btnStates
    for (unsigned int i = 0; i < 4; i++)
    {
        btnStates[i] = false;
    }

    //Re-enable the interrupts.
    enableInterrupts();
}

#pragma endregion

#pragma region ISRs

ISR(PCINT0_vect)
{
    interruptAction();
}

ISR(PCINT2_vect)
{
    interruptAction();
}

#pragma endregion

#endif