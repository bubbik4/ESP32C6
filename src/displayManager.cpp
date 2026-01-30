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
}