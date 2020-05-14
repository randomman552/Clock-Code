#include "SevenSegHandler.h"

#define MAX_BRIGHTNESS 15.0F
#pragma region Constructors

SevenSegHandler::SevenSegHandler(int dataPin, int clkPin, int loadPin, int brightness)
    //Intantiate the LEDControl object
    : _lc(dataPin, clkPin, loadPin, 1)
{
    //Prevent controller from sleeping
    _lc.shutdown(0, false);

    setBrightness(brightness);
}

#pragma endregion

#pragma region Setters and Getters

void SevenSegHandler::setBrightness(int brightness)
{
    _brightness = (int)(MAX_BRIGHTNESS * ((float)brightness / 100));
    _lc.setIntensity(0, _brightness);
}

#pragma endregion

#pragma region Display methods

void SevenSegHandler::print(char toPrint[])
{
    int digit = 0;
    byte dotState = B00000000;
    //Print each character and determine dot states
    for (int i = 0; i < strlen(toPrint); i++)
    {
        //If the next character is a dot, toggle the dotstate of the device
        char nextChar = toPrint[(i + 1) % strlen(toPrint)];

        //Display character
        byte toDisplay = pgm_read_byte_near(allCharTable + toPrint[i]);
        _lc.setRow(0, digit, toDisplay);

        //If there was a dot, skip the next character
        if (nextChar == '.' || nextChar == ':' || nextChar == ',' || nextChar == ';')
        {
            //This sets the bit for the current digit for dotStates.
            //Add 0.5 to prevent integer truncation being a problem
            dotState += pow(2, 8 - (digit + 1)) + 0.5;
            Serial.println(dotState);
            i++;
        }

        //As we are only using 1 MAX7219 chip, we can only display 8 digits before we need to break.
        if (digit > 8)
        {
            break;
        }

        digit++;
    }
    _lc.setColumn(0, 0, dotState);
}

void SevenSegHandler::print(char toPrint[], bool doClear)
{
    if (doClear)
    {
        clear();
    }
    print(toPrint);
}

void SevenSegHandler::displayTime(DateTime time, char format[])
{
    //Define char string, give it a length equal to the maximum possible string generated the format string
    char toPrint[9] = {0};

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
                    char temp[j - i] = {0};
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
                    else if (strcmp(temp, "dow") == 0)
                    {
                        char buffer[10] = {0};
                        strcpy_P(buffer, (char *)pgm_read_word(&(dayofWeek[time.dayOfTheWeek() - 1])));
                        strncat(toPrint, buffer, 3);
                    }
                    else if (strcmp(temp, "smonth") == 0)
                    {
                        char buffer[10] = {0};
                        strcpy_P(buffer, (char *)pgm_read_word(&(monthofYear[time.month() - 1])));
                        strncat(toPrint, buffer, 3);
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

void SevenSegHandler::clear()
{
    _lc.clearDisplay(0);
}

#pragma endregion

#pragma region Misc methods

String SevenSegHandler::_digitFormatter(String toFormat)
{
    if (toFormat.length() < 2)
    {
        toFormat = "0" + toFormat;
    }
    return toFormat;
}

#pragma endregion

//Undefine MAX_BRIGHTNESS so it doesn't interfere with other code
#undef MAX_BRIGHTNESS