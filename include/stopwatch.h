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
};