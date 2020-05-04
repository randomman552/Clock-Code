# Clock Code

This is improved code for an arduino powered clock I am working on, you are free to use snippets of it if you want.

The clock I am working on uses a DS3231 RTC for timekeeping, a MAX7219 and a 4 digit 7 segment display to display the time, and an LED strip compatible with the FastLED library.

External librarys used:
FastLED     - For the LED strip
LedControl  - For the 7 segment display
RTClib      - For control of the RTC