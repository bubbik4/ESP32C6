#pragma once
/*
Device will be powered from a Li-Ion 4.2v battery. class PowerManager will implement calculating percentage and voltage.
The battery's positive terminal is connected to ESP via two resistors @A0 PIN. Use it for measuring. 
Pre-calculated calibration factor, which should be included in calculation -> 2.61
*/
#include "config.h"
#include <Arduino.h>

class PowerManager {
public:
    PowerManager();

    void begin();
    void update();

    float getVoltage() const { return _voltage; }
    int getPercentage() const { return _percentage; }

private:
    const float _calibrationFactor = 2.08f;

    float _voltage = 0.0f;
    int _percentage = 0;

    static const int _numSamples = 10;
    float _readings[_numSamples];
    int _readIndex = 0;
    float _total = 0;
    float _avg = 0;

    unsigned long _lastUpdate;
};