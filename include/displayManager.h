#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <vector>
#include <string>

#include "config.h"

class DisplayManager {
public:
    DisplayManager();

    void begin();

    void drawBootScreen();
    void drawClock();

    void drawMenu();
    void drawTimer();

    void moveSelection(int dir);

private:
    Adafruit_SSD1306 _display;

    const std::vector<std::string> _menuItems = {
        "Clock",
        "Pomodoro",
        "Stopwatch",
        "WiFi Info",
        "System"
    };

    int _selectedIndex = 0;
    int _scrollOffset = 0;

    const int _headerHeight = 16;
    const int _itemsPerScreen = 4;
    const int _lineHeight = 12;
};