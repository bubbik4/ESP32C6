#include <Arduino.h>

class Button {
private:
    const int _pin;
    int _state;
    int _lastState;
    unsigned long _lastDebounceTime;
    unsigned long _debounceDelay;

    unsigned long _pressStartTime;
    bool _isLongPressHandled;

public:
    Button(int pin, unsigned long debounceDelay = 50) : _pin(pin), _debounceDelay(debounceDelay) {
    _lastState = HIGH;
    _state = HIGH;
    _lastDebounceTime = 0;
    _pressStartTime = 0;
    _isLongPressHandled = false;
    }

    void begin() {
        pinMode(_pin, INPUT_PULLUP);
    }

    void update() {
        int reading = digitalRead(_pin);

        if(reading != _lastState) {
            _lastDebounceTime = millis();
        }

        if((millis() - _lastDebounceTime) > _debounceDelay) {
            if(reading != _state) {
                _state = reading;

                if(_state == LOW) {
                    _pressStartTime = millis();
                    _isLongPressHandled = false;
                }
            }
        }
        _lastState = reading;
    }

    bool isClicked() {
       if(_state == HIGH && _lastState == LOW && !_isLongPressHandled) {
        return false;
       }
       return true;
    }

    bool hasJustClicked() {
        static int prevUiState = HIGH;
        bool click = false;

        if(_state == HIGH && prevUiState == LOW) {
            if(!_isLongPressHandled) click = true;
        }
        prevUiState = _state;
        return click;
    }

    bool hasJustHeld(unsigned long duration = 800) {
        if(_state == LOW && !_isLongPressHandled) {
            if(millis() - _pressStartTime > duration) {
                _isLongPressHandled = true;
                return true;
            }
        }
        return false;
    }
};