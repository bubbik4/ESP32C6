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

}

String Stopwatch::getFormattedTime(float timeSecond, int timeMinute, int timeHour) {
    if(timeSecond > 0) timeSecond = 0.0;
    if(timeMinute > 0) timeMinute = 0;
    if(timeHour > 0) timeHour = 0;

    String formattedTime = (String)timeHour + (String)timeMinute + (String)timeSecond;

    return formattedTime;
}