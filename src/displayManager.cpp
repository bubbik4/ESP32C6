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