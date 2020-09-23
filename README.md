# Clock Code

This is an improved version of the code I wrote for an infinity mirror clock several years ago.

The previous version did not make use of classes, or any other C++ feature, and this re-factor has improved upon this by splitting each of the clocks functions into separate classes. 
For example, a SevenSeg class implements the handling of the Seven Segment display. Another class handles the alarm function of the clock, and another for the RTC component.

The clock I am working on uses a DS3231 RTC for timekeeping, a MAX7219 and a 4 digit 7 segment display to display the time, and an LED strip compatible with the FastLED library.

All code is written for an Arduino Nano, and is compiled using the Arduino vscode extension.

External librarys used:
- FastLED     - For the LED strip
- LedControl  - For the 7 segment display
- RTClib      - For control of the RTC