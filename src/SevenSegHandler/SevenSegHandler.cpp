#include "SevenSegHandler.h"

/*
* The constructor instantiates the LEDControl object with the required variables passed through, 
and sets the default brightness.
* dataPin (int) - The pin connected to the MAX7219 data line.
* clkPin (int) - The pin connected to the MAX7219 clock line.
* loadPin (int) - The pin connected to the MAX7219 load line.
* deviceNum (int) - The number of MAX7219 devices linked together.
* brightness (int) - The brightness to start the MAX7219 at (between 0 and 15).
*/
SevenSegHandler::SevenSegHandler(int dataPin, int clkPin, int loadPin, int brightness)
    //Intantiate the LEDControl object
    : _lc(dataPin, clkPin, loadPin, 1)
{
    //Prevent controller from sleeping
    _lc.shutdown(0, false);

    setBrightness(brightness);
}

/* 
* Set the brightness of the seven segment display 
* Brightness (int) - The brightness to set the display to (0-15).
*/
void SevenSegHandler::setBrightness(int brightness)
{
    _brightness = brightness;
    _lc.setIntensity(0, _brightness);
}

/*
* Function to print a string to the seven segment display, 
if the given string is longer than the display, it will be scrolled across the display.
* toPrint (String) - The string to print.
*/
void SevenSegHandler::print(char toPrint[])
{
    //TODO: Need to update this implementation,
    // currently won't do characters like lower case t
    Serial.print("Printing '");
    Serial.print(toPrint);
    Serial.println("' to seven seg.");

    //Clear the display
    _lc.clearDisplay(0);

    int digit = 0;

    //Print each character and determine dot states
    for (int i = 0; i < strlen(toPrint); i++)
    {
        //If the next character is a dot, toggle the dotstate of the device
        char nextChar = toPrint[(i + 1) % strlen(toPrint)];
        bool dotState = false;
        if (nextChar == '.' || nextChar == ':')
        {
            dotState = true;
        }

        //Display character
        _lc.setChar(0, digit, toPrint[i], dotState);
        digit++;

        //If there was a dot, skip the next character
        if (dotState)
        {
            i++;
        }
    }
}

/* 
* Function to display the given DateTime object on the seven segment display.
* time (DateTime) - The time to display.
* mode (String) - The format to display. e.g. "{hour}:{minute}" would display 01:00 if it was 1 o'clock.
*/
void SevenSegHandler::displayTime(DateTime time, char format[])
{
    Serial.println(format);
    //Define char string, give it a length equal to the maximum possible string generated the format string
    char toPrint[5] = {0};

    //Find the position of a { and the corresponding }, the value between is the formatting info.
    for (int i = 0; i < strlen(format); i++)
    {
        if (format[i] == '{')
        {
            //Find the closing bracket.
            for (int j = i; j < strlen(format); j++)
            {
                if (format[j] == '}')
                {
                    //Get the substring
                    char temp[j - i];
                    strncpy(temp, format + i + 1, j - i - 1);
                    temp[j - i - 1] = '\0';

                    //Digit buffer
                    char digitBuf[3] = {0};

                    //TODO: Probably a better way of doing this
                    if (strcmp(temp, "min") == 0 || strcmp(temp, "minute") == 0)
                    {
                        //Get minute as a string
                        String minute = _digitFormatter((String)time.minute());

                        //Convert to char array
                        minute.toCharArray(digitBuf, 3);

                        //Add it to toPrint
                        strcat(toPrint, digitBuf);
                    }
                    else if (strcmp(temp, "hour") == 0)
                    {
                        String hour = _digitFormatter((String)time.hour());
                        hour.toCharArray(digitBuf, 3);
                        strcat(toPrint, digitBuf);
                    }
                    else if (strcmp(temp, "sec") == 0 || strcmp(temp, "second") == 0)
                    {
                        String second = _digitFormatter((String)time.second());
                        second.toCharArray(digitBuf, 3);
                        strcat(toPrint, digitBuf);
                    }
                    else if (strcmp(temp, "day") == 0)
                    {
                        String day = _digitFormatter((String)time.day());
                        day.toCharArray(digitBuf, 3);
                        strcat(toPrint, digitBuf);
                    }
                    else if (strcmp(temp, "month") == 0)
                    {
                        String month = _digitFormatter((String)time.month());
                        month.toCharArray(digitBuf, 3);
                        strcat(toPrint, digitBuf);
                    }
                    else if (strcmp(temp, "year") == 0)
                    {
                        String year = (String)time.year();
                        year.toCharArray(digitBuf, 3, 2);
                        strcat(toPrint, digitBuf);
                    }

                    //Update i (to skip over the chars in the string) and break.
                    i = j;
                    break;
                }
            }

        } //If it is not a value to replace, add it to the print String.
        else
        {
            //Copy the relevant character.
            char charBuf[2] = {0};
            strncpy(charBuf, format + i, 1);
            strcat(toPrint, charBuf);
        }
    }
    print(toPrint);
}

/*
* This function ensures that the time given to it as a string is given to 2 digit presision.
* toFormat (String) - The string to format
* return (String) - The string with a guarenteed 2 digits
*/
String SevenSegHandler::_digitFormatter(String toFormat)
{
    if (toFormat.length() < 2)
    {
        toFormat = "0" + toFormat;
    }
    return toFormat;
}