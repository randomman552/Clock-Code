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
SevenSegHandler::SevenSegHandler(int dataPin, int clkPin, int loadPin, int deviceNum, int brightness)
    //Intantiate the LEDControl object
    : _lc(dataPin, clkPin, loadPin, deviceNum)
{
    //Store the passed values for later checks
    _deviceNum = deviceNum;

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
void SevenSegHandler::print(String toPrint)
{
    byte dotState = B00000000;
    int digit = 0;
    for (int i = 0; i < toPrint.length(); i++)
    {
        //TODO: Need to update this implementation,
        // currently won't do characters like lower case t
        _lc.setChar(0, digit, toPrint[i], false);
        digit++;

        if (toPrint[i + 1] == '.')
        {
            //Skip one interation
            i++;
            dotState += B1000000;
        }

        //Right shift the dotState
        dotState = dotState >> 1;
    }
    _lc.setColumn(0, 0, dotState);
}

/* 
* Function to display the given DateTime object on the seven segment display.
* time (DateTime) - The time to display.
* mode (String) - The format to display. e.g. "{hour}:{minute}" would display 01:00 if it was 1 o'clock.
*/
void SevenSegHandler::displayTime(DateTime time, String format)
{
    String toPrint = "";

    //Find the position of a { and the corresponding }, the value between is the formatting info.
    for (int i; i < format.length(); i++)
    {
        if (format[i] == '{')
        {
            //Find the closing bracket.
            for (int j = i; j < format.length(); j++)
            {
                if (format[j] == '}')
                {
                    //Get the substring and convert to lower case
                    String temp = format.substring(i + 1, j);
                    temp.toLowerCase();

                    //TODO: Probably a better way of doing this
                    if (temp == "min" || temp == "minute")
                    {
                        toPrint += _digitFormatter((String)time.minute());
                    }
                    else if (temp == "hour")
                    {
                        toPrint += _digitFormatter((String)time.hour());
                    }
                    else if (temp == "sec" || temp == "second")
                    {
                        toPrint += _digitFormatter((String)time.second());
                    }
                    else if (temp == "day")
                    {
                        toPrint += _digitFormatter((String)time.day());
                    }
                    else if (temp == "month")
                    {
                        toPrint += _digitFormatter((String)time.month());
                    }
                    //Month as string
                    else if (temp.substring(0, 7) == "smonth")
                    {
                        int limit = temp.substring(7).toInt();
                        toPrint += monthofYear[time.month()].substring(0, limit);
                    }
                    //Day of week as string
                    else if (temp.substring(0, 3) == "dow")
                    {
                        int limit = temp.substring(3).toInt();
                        toPrint += dayofWeek[time.dayOfTheWeek()].substring(0, limit);
                    }

                    //Break out of the inner loop
                    break;
                }
            }

        } //If it is not a value to replace, add it to the print String.
        else
        {
            toPrint += format[i];
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
}