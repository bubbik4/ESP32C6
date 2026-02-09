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
        timeSecond -= 60.00;
        timeMinute++;
    }

    if(timeMinute >= 60) {
        timeMinute -= 60;
        timeHour++;
    }

    if(millis() - lastUpdate > millisInterval) {
        lastUpdate = millis();
        LOG(getFormattedTime());
    }
}

String Stopwatch::getFormattedTime() {
    String sHour = (timeHour < 10) ? "0" + String(timeHour) : String(timeHour);
    String sMin  = (timeMinute < 10) ? "0" + String(timeMinute) : String(timeMinute);
   
    return sHour + ":" + sMin;
}

float Stopwatch::getSeconds() {
    return timeSecond;
}