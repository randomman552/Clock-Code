#include "millisDelay.h"

extern void millisDelay(long duration)
{
    long startTime = millis();

    while (millis() < startTime + duration)
    {
    }
}