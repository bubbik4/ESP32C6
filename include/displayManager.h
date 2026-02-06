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

    // seperate oled tabs
    void drawBootScreen();
    void drawMenu();
    void drawClock();
    void drawPomodoro(String time, String statusLabel, int count);
    void drawStopwatch();
    void drawWiFiInfo(String ssid, String ip, String mac, int rssi, bool connected);
    void drawSystem(float voltage, int percent, size_t usedBytes, size_t totalBytes, size_t appUsed, size_t appTotal);
    void drawWIP();
    void drawConfirmation(String message, bool yesSelected);

    void updateClock();

    void moveSelection(int dir);
    void invertScreen(bool invert);

    int getSelectedIndex() const { return _selectedIndex; }

private:
    Adafruit_SSD1306 _display;
    void drawProgressBar(int x, int y, int width, int height, int percent);

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