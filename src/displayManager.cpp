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

void DisplayManager::updateClock() {
    static unsigned long lastUpdate = 0;

    if(millis() - lastUpdate > 1000) {
        lastUpdate = millis();
        drawClock();
    }
}

void DisplayManager::drawMenu() {
    _display.clearDisplay();

    _display.drawLine(0, _headerHeight - 1, SCR_WIDTH, _headerHeight - 1, SSD1306_WHITE);

    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 4);
    _display.print("   MAIN MENU   ");


    for (int i = 0; i < _itemsPerScreen; i++) {
        int itemIndex = _scrollOffset + i;
        
        if (itemIndex >= _menuItems.size()) break;

        int yPosition = _headerHeight + (i * _lineHeight);

        if (itemIndex == _selectedIndex) {
            _display.fillRect(0, yPosition, SCR_WIDTH, _lineHeight, SSD1306_WHITE);

            _display.setTextColor(SSD1306_BLACK);
        } else {
            _display.setTextColor(SSD1306_WHITE);
        }

        _display.setCursor(5, yPosition + 2);
        _display.print(_menuItems[itemIndex].c_str());
    }

    _display.display();
}

void DisplayManager::moveSelection(int dir) {   
    _selectedIndex += dir;

    if(_selectedIndex < 0) {
        _selectedIndex = _menuItems.size() - 1;

        _scrollOffset = _menuItems.size() - _itemsPerScreen;
        if(_scrollOffset < 0) _scrollOffset = 0;
    }
    else if(_selectedIndex >= _menuItems.size()) {
        _selectedIndex = 0;
        _scrollOffset = 0;
    }

    if(_selectedIndex >= _scrollOffset + _itemsPerScreen) {
        _scrollOffset = _selectedIndex - _itemsPerScreen + 1;
    }

    if(_selectedIndex < _scrollOffset) {
        _scrollOffset = _selectedIndex;
    }

    drawMenu();
}

void DisplayManager::drawWIP() {
    _display.clearDisplay();
    _display.setCursor(0, 64/2);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.print("Work in progress...");

    _display.display();
}