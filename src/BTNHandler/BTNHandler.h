#ifndef BTNHandler_h
#define BTNHandler_h

#include <Arduino.h>

//This tutorial was very helpful for the interupts
//https://www.teachmemicro.com/arduino-interrupt-tutorial/

//Define button pins
#define BTN1 8
#define BTN2 7
#define BTN3 6
#define BTN4 5

bool lowPowerMode = false;

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

void btnAction()
{
    //Get the states of all the buttons when this is called.
    bool btn1 = digitalRead(BTN1);
    bool btn2 = digitalRead(BTN2);
    bool btn3 = digitalRead(BTN3);
    bool btn4 = digitalRead(BTN4);
    Serial.println((String)btn1 + " " + (String)btn2 + " " + (String)btn3 + " " + (String)btn4);

    //Disable interupts while this function runs
    PCICR = B00000000;

    //If any of the buttons are pressed
    if (btn1 || btn2 || btn3 || btn4)
    {
        //Play a tone for the user if at least one of the button is active.
        tone(BUZZER, 60, 100);

        if (btn1 && btn2)
        {
            /* SET ALARM */
        }
        else if (btn3 && btn4)
        {
            /* SET TIME */
        }
        else if (btn1 && btn3)
        {
            /* CHANGE CUSTOM COLOR */
        }
        else if (btn2 && btn4)
        {
            /* EMPTY BIND */
        }
        else if (btn1)
        {
            //If the clock is already in low power mode disable it, otherwise enable it
            if (lowPowerMode)
            {
                uint8_t brightness = EEPROM.read(BRIGHTNESS_STORE);
                SevenSeg.setBrightness(brightness);
                LEDStrip.setBrightness(brightness);
                lowPowerMode = !lowPowerMode;
                SevenSeg.print("l-on");
            }
            else
            {
                SevenSeg.setBrightness(70);
                LEDStrip.setBrightness(0);
                lowPowerMode = !lowPowerMode;
                SevenSeg.print("loff");
            }
        }
        else if (btn2)
        {
            /* ALARM OFF */
        }
        else if (btn3)
        {
            /*Increase the effect on the LED strip by 1, 
            then modulus by the max effect + 1 to wrap it around to 0 if above the max value.*/
            LEDStrip.setEffect((LEDStrip.getEffect() + 1) % (LEDStrip.getMaxEffect() + 1));

            //Show feedback to the user
            SevenSeg.print("ledu");
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

            //Show feedback to the user
            SevenSeg.print("led-");
        }
    }

    //Block until all buttons are released
    while (btn1 || btn2 || btn3 || btn4)
    {
        btn1 = digitalRead(BTN1);
        btn2 = digitalRead(BTN2);
        btn3 = digitalRead(BTN3);
        btn4 = digitalRead(BTN4);
    }

    //Re-enable the interupts
    PCICR = B00000101;
}

ISR(PCINT0_vect)
{
    btnAction();
}

ISR(PCINT2_vect)
{
    btnAction();
}

#endif