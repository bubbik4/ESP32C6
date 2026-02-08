#pragma once

#include "config.h"

#include <Arduino.h>

class Stopwatch {
public:
    Stopwatch();

    void begin();
    void togglePause();
    void updateTimer();

    String getFormattedTime();

private:

    float timeSecond;
    int timeMinute;
    int timeHour;

    bool isPaused;

    unsigned long lastUpdate = 0;
    const int millisInterval = 1;
};