#ifndef SevenSeg_h
#define SevenSeg_h

#include <Arduino.h>
#include <LedControl.h>
#include <RTClib.h>

#define MAX_BRIGHTNESS 15.0F

#pragma region PROGMEM character bytes

/*
* A series of bytes stored in PROGMEM, 
these are used to store how certain characters are displayed on the seven segment display.
* They are stored in PROGMEM in ascii order, starting from the space character.
* This doesn't override the standard charTable used by LEDControl (could fork the library and edit it for this).
*/
static const byte allCharTable[] PROGMEM = {
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

/**
* Extension of the LedControl class.
* Contains methods useful for displaying time to the Seven Segment display.
*/
class SevenSeg : public LedControl
{
private:
    int _brightness;

public:
#pragma region Constructors

    /**
     * Specifies the pins used to communicated with the MAX7219
     * dataPin (int) - The pin used for data
     * clkPin (int) - The pin used for clock
     * csPin (int) - The pin used for load/cs
     * numDevices (int) - The number of MAX7219 chips in use
     */
    SevenSeg(int dataPin, int clkPin, int csPin, int numDevices);

#pragma endregion

#pragma region Display methods

    /**
     * Print the given null-terminated string to the display.
     * Will deal with characters such as '.', ':', ';', ',' by activating the required DP segment.
     * toPrint (char[]) - The string to print.
     */
    void print(const char toPrint[]);

    /**
     * Print the given null-terminated string to the display.
     * Will deal with characters such as '.', ':', ';', ',' by activating the required DP segment.
     * toPrint (char[]) - The string to print.
     * doClear (bool) - Whether to clear the display before printing to the display.
     */
    void print(const char toPrint[], const bool doClear);

    /**
     * Works in a similar way to python f strings.
     * For example, printf("{0}:{1}", {"01", "02"}) would display "01:02" on the seven segment display.
     */
    void printf(const char format[], const char *values[]);

#pragma endregion
};

/**
 * This function takes an integer and converts it to a null terminated char array of the given length.
 * All digits afteer the given length will be truncated.
 * This is intended for use with the SevenSeg.printf method.
 * toFormat (int) - The integer to turn into a string.
 * dest (char*) - Where to store the resulting string.
 * len (int) - The desired length of the resulting string.
 * return (char*) - Pointer to the destination array.
 */
char *formatInt(const int toFormat, char *dest, const int len);

#endif