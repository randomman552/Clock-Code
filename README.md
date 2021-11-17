# Clock Code
<img src=".github/preview.gif" style="width: 100%"/>

This is an improved version of the code I wrote for an infinity mirror clock several years ago (see above picture).\
Uses a DS3231 RTC for timekeeping, a MAX7219 and a 4 digit 7 segment display to display the time, and an LED strip compatible with the FastLED library.

External librarys used:
- [LED Strip Controller](https://github.com/randomman552/Led-Strip-Controller) - For the LED Strip (made by me)
    - [FastLED](https://www.arduino.cc/reference/en/libraries/fastled/)
    - [SerialCommands](https://www.arduino.cc/reference/en/libraries/serialcommands/)
- [LedControl](https://www.arduino.cc/reference/en/libraries/ledcontrol/) - For the 7 segment display
- [RTClib](https://www.arduino.cc/reference/en/libraries/rtclib/) - For control of the RTC
