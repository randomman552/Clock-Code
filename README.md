# Clock Code
This is an improved version of the code I wrote for an infinity mirror clock several years ago.\
The clock I am working on uses a DS3231 RTC for timekeeping, a MAX7219 and a 4 digit 7 segment display to display the time, and an LED strip compatible with the FastLED library.

External librarys used:
- (https://github.com/randomman552/Led-Strip-Controller)[LED Strip Controller] - For the LED Strip (made by me)
    - (https://www.arduino.cc/reference/en/libraries/fastled/)[FastLED]
    - (https://www.arduino.cc/reference/en/libraries/serialcommands/)[SerialCommands]
- (https://www.arduino.cc/reference/en/libraries/ledcontrol/)[LedControl] - For the 7 segment display
- (https://www.arduino.cc/reference/en/libraries/rtclib/)[RTClib] - For control of the RTC
