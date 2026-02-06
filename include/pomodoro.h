#pragma once
#include <Arduino.h>

enum PomodoroState {
    POM_IDLE,
    POM_WORK,
    POM_BREAK,
    POM_PAUSED
};

class PomodoroTimer {
public:
    PomodoroTimer();

    void toggleStartPause();

    void reset();

    void update();

    String getFormattedTime();
    PomodoroState getState() { return _state; }

    void switchState();

    bool isAlarmTriggered() {
        if(_alarmTriggered) {
            _alarmTriggered = false;
            return true;
        }
        return false;
    }

    int getCycleCount() const { return _cycleCount; }

    String getStatusLabel();

private:
    PomodoroState _state;
    PomodoroState _prevState;
    bool _alarmTriggered = false;

    const int _workDurationMinutes = 25;
    const int _breakDurationMinutes = 5;
    const int _longBreakDurationMinutes = 15;
    
    int _cycleCount = 0;

    unsigned long _remainingTimeSeconds = 0;
    unsigned long _lastTickTime;

    void startWork();
    void startBreak();

};