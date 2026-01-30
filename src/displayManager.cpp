#include "displayManager.h"

DisplayManager::DisplayManager() 
    : _display(SCR_WIDTH, SCR_HEIGHT, &Wire, -1) {}

void DisplayManager::begin() {
    Wire.begin(SCR_SDA, SCR_SCL);

    if(!_display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        LOG("SSD1306 allocation failed");
        for(;;);
    }
    _display.clearDisplay();
    _display.display();
}

void DisplayManager::drawBootScreen() {
    _display.clearDisplay();

    _display.fillRect(0, 0, 128, 16, SSD1306_WHITE);
    _display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    _display.setTextSize(2);
    _display.setCursor(10, 1);
    _display.print("CYBERDECK");

    _display.display();
    delay(500);
    _display.setTextColor(SSD1306_WHITE, SSD1306_BLACK);
}

void DisplayManager::drawClock() {
    struct tm timeinfo;
    if(getLocalTime(&timeinfo)) {
        _display.clearDisplay();
        _display.setTextSize(2);
        _display.setCursor(16,25);
        _display.printf("%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

        _display.setTextSize(1);
        _display.setCursor(25,48);
        char timeStr[30];
        strftime(timeStr, sizeof(timeStr), "%a, %d, %b", &timeinfo);
    } else {
        _display.println("Syncing time");
        
    }_display.display();
}