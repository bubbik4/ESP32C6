#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include "config.h"

class DisplayManager {
public:
    DisplayManager();
    
    void begin();

private:
    Adafruit_SSD1306 _display;

    
};