#ifndef millisDelay_h
#define millisDelay_h

/*
* An implementation of delay, but using millis to make it non-blocking.
* duration (long) - The period of time to delay for (in milliseconds).
*/
void millisDelay(long duration)
{
    long startTime = millis();

    while (millis() < startTime + duration)
    {
    }
}

#endif