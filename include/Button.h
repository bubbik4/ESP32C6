#include <Arduino.h>

class Button {
private:
    const int _pin;
    int _state;
    int _lastState;
    unsigned long _lastDebounceTime;
    unsigned long _debounceDelay;

public:
    Button(int pin, unsigned long debounceDelay = 50) : _pin(pin), _debounceDelay(debounceDelay) {
    _lastState = HIGH;
    _state = HIGH;
    _lastDebounceTime = 0;
    }

    void begin() {
        pinMode(_pin, INPUT_PULLUP);
    }

    bool isClicked() {
        int reading = digitalRead(_pin);
        bool clicked = false;

        if(reading != _lastState) {
            _lastDebounceTime = millis();
        }
        
        if((millis() - _lastDebounceTime) > _debounceDelay) {
            if(reading != _state) {
                _state = reading;
                if(_state == LOW) {
                    clicked = true;
                }
            }
        }
        _lastState = reading;
        return clicked;
    }
};