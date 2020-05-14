
#ifndef SevenSegHandler_h
#define SevenSegHandler_h

#include <Arduino.h>
#include <LedControl.h>
#include <RTClib.h>

#pragma region PROGMEM month

//Store months of year in PROGMEM
const char monthofYear1[] PROGMEM = "january";
const char monthofYear2[] PROGMEM = "february";
const char monthofYear3[] PROGMEM = "march";
const char monthofYear4[] PROGMEM = "april";
const char monthofYear5[] PROGMEM = "may";
const char monthofYear6[] PROGMEM = "june";
const char monthofYear7[] PROGMEM = "july";
const char monthofYear8[] PROGMEM = "august";
const char monthofYear9[] PROGMEM = "september";
const char monthofYear10[] PROGMEM = "october";
const char monthofYear11[] PROGMEM = "november";
const char monthofYear12[] PROGMEM = "december";

//Create table to refer to the month strings
const char *const monthofYear[] PROGMEM = {monthofYear1, monthofYear2, monthofYear3, monthofYear4,
                                           monthofYear5, monthofYear6, monthofYear7, monthofYear8,
                                           monthofYear9, monthofYear10, monthofYear11, monthofYear12};

#pragma endregion

#pragma region PROGMEM days of week
//Store days of week in PROGMEM
const char dayofWeek1[] PROGMEM = "sunday";
const char dayofWeek2[] PROGMEM = "monday";
const char dayofWeek3[] PROGMEM = "tuesday";
const char dayofWeek4[] PROGMEM = "wednesday";
const char dayofWeek5[] PROGMEM = "thursday";
const char dayofWeek6[] PROGMEM = "friday";
const char dayofWeek7[] PROGMEM = "saturday";

//Create table to refer to the day strings
const char *const dayofWeek[] PROGMEM = {dayofWeek1, dayofWeek2, dayofWeek3, dayofWeek5,
                                         dayofWeek6, dayofWeek7};

#pragma endregion

#pragma region PROGMEM character bytes

/*
* A series of bytes stored in PROGMEM, 
these are used to store how certain characters are displayed on the seven segment display.
* They are stored in PROGMEM in ascii order, starting from the space character.
* This doesn't override the standard charTable used by LEDControl (could fork the library and edit it for this).
*/
const byte allCharTable[] PROGMEM = {
    B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, // 000-007 :
    B01111111, B01111011, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, // 008-015 :
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // 016-023 :
    B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, // 024-031 :
    B00000000, B00000110, B00100010, B01111111, B01011011, B01011010, B01011110, B00000010, // 032-039 :
    B01001110, B01111000, B01000001, B00110001, B10000000, B00000001, B10000000, B00100101, // 040-047 :
    B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, // 048-055 : 0-7
    B01111111, B01111011, B00010010, B00011010, B01000011, B00001001, B01100001, B01100101, // 056-063 : 8-?
    B01101110, B01110111, B00011111, B01001110, B00111101, B01001111, B01000111, B01011110, // 064-071 : @-G
    B00110111, B00110000, B00111100, B01010111, B00001110, B01101010, B01110110, B01111110, // 072-079 : H-O
    B01100111, B01110011, B00000101, B01011011, B00001111, B00111110, B00111110, B01011100, // 080-087 : P-W
    B01001001, B00111011, B01101101, B01001110, B00010011, B01111000, B01100010, B00001000, // 088-095 : X-_
    B01100000, B01110111, B00011111, B00001101, B00111101, B01001111, B01000111, B01011110, // 096-103 : `-g
    B00110111, B00010000, B00111100, B01001011, B00001110, B01010100, B00010101, B00011101, // 104-113 : h-o
    B01100111, B01110011, B00000101, B01011011, B00001111, B00011100, B00100011, B00101010, // 114-119 : p-w
    B00001001, B00111011, B01101101, B01001110, B00000110, B01111000, B00000001, B00000000  // 120-127 : x-[del]
};
#pragma endregion

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