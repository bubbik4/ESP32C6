// powermanager.cpp
#include "powerManager.h"

PowerManager::PowerManager() {}

void PowerManager::begin() {
    pinMode(BATTERY_PIN, INPUT);

    for(int i = 0; i < _numSamples; i++) {
        _readings[i] = 0;
    }
}

void PowerManager::update() {
    if(millis() - _lastUpdate < 100) return;
    _lastUpdate = millis();

    float rawMilliVolts = analogReadMilliVolts(BATTERY_PIN);

    _total = _total - _readings[_readIndex];
    _readings[_readIndex] = rawMilliVolts;
    _total = _total + _readings[_readIndex];
    _readIndex++;
    if(_readIndex >= _numSamples) _readIndex = 0;
    _avg = _total / _numSamples;

    _voltage = (_avg / 1000.0f) * _calibrationFactor;

    int pct = map((long)(_voltage * 100), 320, 420, 0 ,100);
    
    if (pct > 100) pct = 100;
    if (pct < 0) pct = 0;

    _percentage = pct;
}