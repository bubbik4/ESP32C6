#include "stopwatch.h"

Stopwatch::Stopwatch() {}

void Stopwatch::begin() {
    timeSecond = 0.0;
    timeMinute = 0;
    timeHour   = 0;
    isPaused = true;
    startTime = millis();
    lastCalculationTime = millis();
}

void Stopwatch::togglePause() {
    isPaused = !isPaused;
    if(!isPaused) {
        lastCalculationTime = millis();
    }
    LOG("Stopwatch toggled");
}

void Stopwatch::updateTimer() {
    unsigned long currentMillis = millis();
    if (!isPaused) {
        unsigned long dt = currentMillis - lastCalculationTime;
        timeSecond += dt / 1000.0;
        lastCalculationTime = currentMillis;
    } else {
        lastCalculationTime = currentMillis;
    }
    
    if(timeSecond >= 60.00) {
        timeSecond = 0.00;
        timeMinute++;
    }

    if(timeMinute >= 60) {
        timeMinute = 0;
        timeHour++;
    }

    if(millis() - lastUpdate > millisInterval) {
        lastUpdate = millis();
        LOG(getFormattedTime());
    }
}

String Stopwatch::getFormattedTime() {
    if(timeSecond < 0) timeSecond = 0.0;
    if(timeMinute < 0) timeMinute = 0;
    if(timeHour < 0) timeHour = 0;

    String formattedTime = (String)timeHour + ":" + (String)timeMinute + ":" + (String)timeSecond;

    return formattedTime;
}