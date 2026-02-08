#include "stopwatch.h"

Stopwatch::Stopwatch() {}

void Stopwatch::begin() {
    timeSecond = 0.0;
    timeMinute = 0;
    timeHour   = 0;
    isPaused = true;
}

void Stopwatch::togglePause() {
    isPaused = !isPaused;
    LOG("Stopwatch toggled");
}

void Stopwatch::updateTimer() {
    if (isPaused) {
        return;
    }

    if(millis() - lastUpdate > millisInterval) {
        lastUpdate = millis();
        timeSecond+=0.01;
        LOG(getFormattedTime());
    }

    if(timeSecond >= 60.00) {
        timeSecond = 0.00;
        timeMinute++;
    }

    if(timeMinute >= 60) {
        timeMinute = 0;
        timeHour++;
    }
}

String Stopwatch::getFormattedTime() {
    if(timeSecond < 0) timeSecond = 0.0;
    if(timeMinute < 0) timeMinute = 0;
    if(timeHour < 0) timeHour = 0;

    String formattedTime = (String)timeHour + ":" + (String)timeMinute + ":" + (String)timeSecond;

    return formattedTime;
}