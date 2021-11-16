#include "Clock.h"

#pragma region Constructors/destructors

Clock::Clock():
    _rtc(),
    _ledStrip(),
    _sevenSeg(12, 11, 10, 1)
{
    
}

Clock::~Clock() { disableInterrupts(); }

void Clock::begin() {
    // Seven seg setup
    _sevenSeg.setIntensity(0, (_ledStrip.getBrightness()/256.0F)*15.0F);
    _sevenSeg.print("Helo");

    // RTC setup
    _rtc.begin();

    // Setup pins for input
    for (int i = 0; i < 4; i++) {
        pinMode(_btnPins[i], INPUT);
    }

    // Setup feedback pin
    pinMode(_feedbackPin, OUTPUT);

    // Interrupt setup
    enableInterrupts();
    // Clear the PCINT flag register
    PCIFR = B00000000;
    // Enable the interupt for pins D0 - D7 and D8 - D13 (for the respective bits)
    PCICR = B00000101;
    // Set the pin masks so that only pin 5-8 are used for interupting.
    PCMSK0 = B00000001;
    PCMSK2 = B11100000;
}

#pragma endregion

#pragma region Button related methods

void Clock::onInterrupt()
{
    // Clear flag register
    PCIFR = B00000000;

    // Give the user a grace period
    // It is almost impossible for the user to press multiple buttons at exactly the same time
    for (int _ = 0; _ < 100; _++) {
        // Update button states
        for (int i = 0; i < 4; i++) {
            _btns[i] = digitalRead(_btnPins[i]);
        }
        delayMicroseconds(1000);
    }

    // Block until buttons are released
    //while (digitalRead(_btnPins[0]) || digitalRead(_btnPins[1]) || digitalRead(_btnPins[2]) || digitalRead(_btnPins[3])){}
    
}

void Clock::onPressed()
{
    // This method should be split into some smaller methods to make it easier to read.
    // The variables here consume quite a lot of memory and could be moved into PROGMEM.
    const bool toggleLights[] = {true, false, false, false};
    const bool toggle12Hour[] = {false, true, false, false};
    const bool nextEffect[] = {false, false, true, false};
    const bool prevEffect[] = {false, false, false, true};

    const bool changeTime[] = {false, false, true, true};
    const bool changeColor[] = {true, true, false, false};

    if (arrayEqual(_btns, toggleLights, 4))
    {
        _ledStrip.setEnabled(!_ledStrip.getEnabled());
        if (_ledStrip.getEnabled())
        {
            _sevenSeg.print("-on-");
        }
        else
        {
            _sevenSeg.print("off-");
        }
        delay(1000);
    }
    else if (arrayEqual(_btns, toggle12Hour, 4))
    {
        _12Hour = !_12Hour;
        if (_12Hour) _sevenSeg.print("12.hr");
        else _sevenSeg.print("24.hr");
        delay(1000);
    }
    else if (arrayEqual(_btns, nextEffect, 4))
    {
        uint8_t curEffect = _ledStrip.getEffect();
        int maxEffect = (sizeof(LEDStripController::lFuncs) / sizeof(LEDStripController::lFuncs[0]));
        _ledStrip.setEffect((curEffect + 1) % maxEffect);
        _sevenSeg.print("led+");
        delay(1000);
    }
    else if (arrayEqual(_btns, prevEffect, 4))
    {
        uint8_t curEffect = _ledStrip.getEffect();
        int maxEffect = (sizeof(LEDStripController::lFuncs) / sizeof(LEDStripController::lFuncs[0]));
        _ledStrip.setEffect((curEffect - 1) % maxEffect);
        _sevenSeg.print("led-");
        delay(1000);
    }
    else if (arrayEqual(_btns, changeTime, 4))
    {
        DateTime time = _rtc.now();
        _sevenSeg.print("c-ti");
        delay(1000);

        // Adjust each element of current time in sequence
        _sevenSeg.print("year");
        delay(1000);
        const int year = changeValue(_rtc.now().year(), 2000, 2100);

        _sevenSeg.print("mnth");
        delay(1000);
        const int month = changeValue(time.month(), 1, 13);

        _sevenSeg.print("day", true);
        delay(1000);
        const int day = changeValue(time.day(), 1, 31);

        _sevenSeg.print("hour");
        delay(1000);
        const int hour = changeValue(time.hour(), 0, 24);

        _sevenSeg.print("min", true);
        delay(1000);
        const int minute = changeValue(time.minute(), 0, 60);
        
        DateTime newTime(year, month, day, hour, minute, 0);
        _rtc.adjust(newTime);
    }
    else if (arrayEqual(_btns, changeColor, 4))
    {
        CRGB color = _ledStrip.getColor();
        _sevenSeg.print("ledc");
        delay(1000);

        // Adjust red, green, and blue values
        _sevenSeg.print("red", true);
        delay(1000);
        color.r = changeValue(color.r, 0, 256);

        _sevenSeg.print("gren");
        delay(1000);
        color.g = changeValue(color.g, 0, 256);

        _sevenSeg.print("blue", true);
        delay(1000);
        color.b = changeValue(color.b, 0, 256);

        _ledStrip.setColor(color);

        // Adjust brightness
        _sevenSeg.print("brit");
        delay(1000);
        const int brightness = changeValue(_ledStrip.getBrightness(), 0, 256);
        _ledStrip.setBrightness(brightness);
        _sevenSeg.setIntensity(0, ((brightness)/256.0F)*15.0F);
    }

    clearButtons();
}

int Clock::changeValue(const int startVal, int min, int max)
{
    int val = startVal;
    char strBuffer[5] = {0};

    // Define button binds
    const bool valUp[] = {false, false, true, false};
    const bool valDown[] = {false, false, false, true};
    const bool confirm[] = {true, false, false, false};
    const bool cancel[] = {false, true, false, false};

    // Clear the buttons to prevent any key-presses from carrying into this method.
    clearButtons();

    // This method continues to utilise the pin change interrupts.
    // As such, we do not need to continuously poll the pins for their state.
    while (true)
    {
        if (buttonPressed()) playFeedback();
        if (arrayEqual(_btns, valUp, 4))
        {
            val += 1;
            if (val >= max) val = min;
        }
        else if (arrayEqual(_btns, valDown, 4))
        {
            val -= 1;
            if (val < min) val = max - 1;
        }
        else if (arrayEqual(_btns, confirm, 4))
        {
            return val;
        }
        else if (arrayEqual(_btns, cancel, 4))
        {
            return startVal;
        }
        
        // Display the value as we change it
        _sevenSeg.print(itoa(val, strBuffer, 10), true);
        // Continue to show led effect as normal
        _ledStrip.mainloop();
    }
    return val;
}

void Clock::clearButtons()
{
    for (int i = 0; i < 4; i++)
    {
        _btns[i] = false;
    }
}

bool Clock::buttonPressed()
{
    for (int i = 0; i < 4; i++) {
        if (_btns[i]) {
            return true;
        }
    }
    return false;
}

void Clock::playFeedback()
{
    tone(_feedbackPin, 60, 100);
}

#pragma endregion

#pragma region Main loop methods

void Clock::displayTime()
{
    static int i = 0;

    // Only query rtc once every few seconds.
    // Many queries in quick succession (such as 60 per second) cause the rtc's time get out of sync.
    if (i >= FastLED.getFPS() * 5)
    {
        i = 0;
        _time = _rtc.now();
    }

    
    char hourStr[3];
    char minStr[3];

    const int hour = (_12Hour) ? _time.twelveHour() : _time.hour();
    const int minute = _time.minute();

    char *values[] = {
        formatInt(hour, hourStr, 2),
        formatInt(minute, minStr, 2)
    };

    _sevenSeg.printf("{0}:{1}", values);

    i++;
}

void Clock::mainloop()
{
    // Check if any buttons are pressed, if they are handle accordingly
    if (buttonPressed())
    {
        playFeedback();
        onPressed();
    }
    displayTime();
    _ledStrip.mainloop();
}

#pragma endregion
