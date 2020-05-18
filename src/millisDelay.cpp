#include "millisDelay.h"

extern void millisDelay(const long duration)
{
    long startTime = millis();

    while (millis() < startTime + duration)
    {
    }
}