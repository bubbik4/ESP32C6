#include "pomodoro.h"

PomodoroTimer::PomodoroTimer() {
    _state = POM_IDLE;
    _prevState = POM_IDLE;
    _remainingTimeSeconds = _workDurationMinutes * 60;
}

void PomodoroTimer::toggleStartPause() {
    switch (_state) {
        case POM_IDLE:
            startWork();
            break;
        case POM_WORK:
        case POM_BREAK:
            _prevState = _state;
            _state = POM_PAUSED;
            break;
        case POM_PAUSED:
            _state = _prevState;
            _lastTickTime = millis();
            break;
    }
}

void PomodoroTimer::startWork() {
    _state = POM_WORK;
    _remainingTimeSeconds = _workDurationMinutes * 60;
    _lastTickTime = millis();
}

void PomodoroTimer::startBreak() {
    _state = POM_BREAK;
    _remainingTimeSeconds = _breakDurationMinutes * 60;
    _lastTickTime = millis();
}

void PomodoroTimer::reset() {
    _state = POM_IDLE;
    _remainingTimeSeconds = _workDurationMinutes * 60;
}

void PomodoroTimer::update() {
    if(_state == POM_IDLE || _state == POM_PAUSED) return;

    if(millis() - _lastTickTime >= 1000) {
        _lastTickTime = millis();

        if(_remainingTimeSeconds > 0) {
            _remainingTimeSeconds--;
        } else {
            _alarmTriggered = true;

            if(_state == POM_WORK) {
                startBreak();
            } else {
                startWork();
            }
        }
    }
}

String PomodoroTimer::getFormattedTime() {
    int minutes = _remainingTimeSeconds / 60;
    int seconds = _remainingTimeSeconds % 60;

    char buffer[6];
    sprintf(buffer, "%02d:%02d", minutes, seconds);
    return String(buffer);
}