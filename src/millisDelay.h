#ifndef millisDelay_h
#define millisDelay_h

#include <Arduino.h>

/*
* An implementation of delay, but using millis to make it non-blocking.
* duration (long) - The period of time to delay for (in milliseconds).
*/
extern void millisDelay(const long duration);

#endif