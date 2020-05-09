
#ifndef SevenSegHandler_h
#define SevenSegHandler_h

#include <Arduino.h>
#include <LedControl.h>
#include <RTClib.h>

//TODO: Should try to make this take up less RAM (move to progmem)
const String dayofWeek[] = {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};
const String monthofYear[] = {"january", "february", "march", "april", "may", "june", "july", "august", "september", "october", "november", "december"};

//This class uses the LEDControl library to display data on a seven segment display.
class SevenSegHandler
{
private:
    int _brightness;
    int _deviceNum;
    LedControl _lc;

    //Functions
    /*
    * This function ensures that the time given to it as a string is given to 2 digit presision.
    * toFormat (String) - The string to format
    * return (String) - The string with a guarenteed 2 digits
    */
    String _digitFormatter(String toFormat);

public:
    //Constructors
    /*
    * This constructor instantiates the LEDControl object with the required variables passed through, 
    and sets the default brightness.
    * dataPin (int) - The pin connected to the MAX7219 data line.
    * clkPin (int) - The pin connected to the MAX7219 clock line.
    * loadPin (int) - The pin connected to the MAX7219 load line.
    * deviceNum (int) - The number of MAX7219 devices linked together.
    * brightness (int) - The brightness to start the MAX7219 at (between 0 and 15).
    */
    SevenSegHandler(int data_pin, int clk_pin, int load_pin, int brightness);

    //Setters
    /* 
    * Set the brightness of the seven segment display 
    * Brightness (int) - The brightness to set the display to (0-15).
    */
    void setBrightness(int newBrightness);

    //Display methods
    /*
    * Method to print a string to the seven segment display, 
    if the given string is longer than the display, it will be scrolled across the display.
    * This will not clear the display, use the "clear" method to do that.
    * toPrint (char[]) - The string to print.
    */
    void print(char toPrint[]);

    /* 
    * Overload of normal print method.
    * toPrint (char[]) - The string to print.
    * doClear (bool) - True will clear the display, then print.
    */
    void print(char toPrint[], bool doClear);

    /* 
    * Function to display the given DateTime object on the seven segment display.
    * time (DateTime) - The time to display.
    * mode (String) - The format to display. e.g. "{hour}:{minute}" would display 01:00 if it was 1 o'clock.
    */
    void displayTime(DateTime time, char format[]);

    /*
    * Clear the display.
    */
    void clear();
};

#endif