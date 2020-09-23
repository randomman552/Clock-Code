#include "SevenSeg.h"

SevenSeg::SevenSeg(int dataPin, int clkPin, int csPin, int numDevices)
    : LedControl(dataPin, clkPin, csPin, numDevices)
{
    //Cancel shutdown
    shutdown(0, false);
}

void SevenSeg::print(const char toPrint[])
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
        setRow(0, digit, toDisplay);

        //If there was a dot, skip the next character
        if (nextChar == '.' || nextChar == ':' || nextChar == ',' || nextChar == ';')
        {
            //This sets the bit for the current digit for dotStates.
            //Add 0.5 to prevent integer truncation being a problem
            dotState += pow(2, 8 - (digit + 1)) + 0.5;
            i++;
        }

        //As we are only using 1 MAX7219 chip, we can only display 8 digits before we need to break.
        if (digit > 8)
        {
            break;
        }

        digit++;
    }
    setColumn(0, 0, dotState);
}

void SevenSeg::print(const char toPrint[], const bool doClear)
{
    if (doClear)
    {
        clearDisplay(0);
    }
    print(toPrint);
}

void SevenSeg::setBrightness(const int brightness)
{
    _brightness = (int)(MAX_BRIGHTNESS * ((float)brightness / 100));
    setIntensity(0, _brightness);
}

void SevenSeg::printf(const char format[], const char *values[])
{
    char toPrint[getDeviceCount() * 8 + 1] = {0};

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

                    strcat(toPrint, values[atoi(temp)]);
                    i = j;
                    break;
                }
            }
        }
        else
        {
            //Copy the current character into the string to print.
            char charBuf[2] = {0};
            strncpy(charBuf, format + i, 1);
            strcat(toPrint, charBuf);
        }
    }
    print(toPrint);
}

char *formatInt(const int toFormat, char *dest, const int len)
{
    //Buffer to store the maximum length string possible (for a 32-bit integer)
    char buffer[33] = {0};
    itoa(toFormat, buffer, 10);

    //Pad with "0" to the left until length requirement is met.
    while (strlen(buffer) < len)
    {
        memmove(buffer + 1, buffer, strlen(buffer));
        buffer[0] = '0';
    }

    //Null terminate the destination string
    dest[len] = 0;

    strncpy(dest, buffer, len);
    return dest;
}
