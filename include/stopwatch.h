#pragma once

#include "config.h"

#include <Arduino.h>

class Stopwatch {
public:
    Stopwatch();

    void begin();
    void togglePause();
    void updateTimer();
    bool getPausedState() const { return isPaused; }

    String getFormattedTime();

private:

    float timeSecond;
    int timeMinute;
    int timeHour;

    bool isPaused;

    unsigned long startTime  = 0;
    unsigned long lastCalculationTime = 0;
    unsigned long lastUpdate = 0;
    const int millisInterval = 40;
};