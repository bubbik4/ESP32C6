#include "displayManager.h"

DisplayManager::DisplayManager() 
    : _display(SCR_WIDTH, SCR_HEIGHT, &Wire, -1) {}

void DisplayManager::begin() {
    Wire.begin(SCR_SDA, SCR_SCL);
    Wire.setClock(400000);

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

void DisplayManager::drawWiFiInfo(String ssid, String ip, String mac, int rssi, bool connected) {
    _display.clearDisplay();

    _display.drawLine(0, _headerHeight - 1, SCR_WIDTH, _headerHeight - 1, SSD1306_WHITE);

    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 4);
    _display.print("WiFi Info");

    int _yStart = _headerHeight + 4;
    int lineStep = 10;

    if(connected) {
        _display.setCursor(0, _yStart);
        _display.print("SSID: ");
        _display.print(ssid);

        _display.setCursor(0, _yStart + lineStep);
        _display.print("IP: ");
        _display.print(ip);

        _display.setCursor(0, _yStart + (lineStep*2));
        _display.print("MAC:");
        _display.print(mac);

        _display.setCursor(0, _yStart +(lineStep * 3));
        _display.printf("Sig: %d dBm", rssi);

        int barWidth = 3;
        int barSpace = 2;
        int xBarStart = 110;
        int yBarBottom = 60;

        int bars = 0;
        if(rssi > -55) bars = 4;
        else if (rssi > -65) bars = 3;
        else if (rssi > -75) bars = 2;
        else if (rssi > -85) bars = 1;

        for (int i = 0; i < 4; i++) {
            int h = (i + 1) * 3;
            if(i < bars) {
                _display.fillRect(xBarStart + (i * (barWidth + barSpace)), yBarBottom - h, barWidth, h, SSD1306_WHITE);
            } else {
                _display.drawRect(xBarStart + (i * (barWidth + barSpace)), yBarBottom - h, barWidth, h, SSD1306_WHITE);
            }
        } 
    } else {
        _display.setCursor(0, 30);
        _display.print("Status: Disconnected");
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

void DisplayManager::invertScreen(bool invert) {
    _display.invertDisplay(invert);
}

void DisplayManager::drawWIP() {
    _display.clearDisplay();
    _display.setCursor(0, 64/2);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.print("Work in progress...");

    _display.display();
}

void DisplayManager::drawPomodoro(String time, String statusLabel, int count) {
    _display.clearDisplay();

    _display.drawLine(0, _headerHeight - 1, SCR_WIDTH, _headerHeight - 1, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(40, 4);
    _display.print("POMODORO");

    _display.setCursor(100, 4);
    _display.print("#");
    _display.print(count);


    _display.setTextSize(3);

    _display.setCursor(20,25);
    _display.print(time);

    _display.setTextSize(1);
    _display.setCursor(0, 54);

    int centerOffset = (21 - statusLabel.length()) * 3;
    if(centerOffset < 0) centerOffset = 0;
    _display.setCursor(centerOffset, 54);

    _display.print(statusLabel);

    _display.display();
}

// helper function for progress bar drawing, to keep simplicity and not copy this 3 times
void DisplayManager::drawProgressBar(int x, int y, int width, int height, int percent) {
    _display.drawRect(x, y, width, height, SSD1306_WHITE);
    if(percent > 0) {
        int barW = map(percent, 0, 100, 0, width);
        _display.fillRect(x, y, barW, height, SSD1306_WHITE);
    }
}

static String formatSize(size_t bytes) {
    if (bytes > 1024 * 1024) {
        return String(bytes / (1024.0 * 1024.0), 2) + "MB";
    }
    return String(bytes / 1024.0, 0) + "KB";
}

void DisplayManager::drawSystem(float voltage, int percent, size_t usedBytes, size_t totalBytes, size_t appUsed, size_t appTotal) {
    _display.clearDisplay();

    _display.drawLine(0, 16 - 1, SCR_WIDTH, 15, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(30, 4);
    _display.print("SYSTEM INFO");
    
    // S1: Battery (y: 17-27)
    
    _display.setCursor(0, 16);
    _display.print("PWR:");

    _display.setTextSize(1);
    _display.setCursor(30, 16);
    _display.print(percent);
    _display.print("%");
    _display.setCursor(60, 16);
    _display.printf("(%.2fV)", voltage);

    drawProgressBar(0, 24, 128, 2, percent);

    // S2: Disk / SPIFFS (y: 30-40)

    _display.setCursor(30, 30);
    _display.print(formatSize(usedBytes));
    _display.print("/");
    _display.print(formatSize(totalBytes));

    int dskPct = (totalBytes > 0) ? (int)((float)usedBytes / totalBytes * 100) : 0;
    if(dskPct > 100) dskPct = 100;
    
    drawProgressBar( 0, 38, 128, 2, dskPct);

    // S3: APP / CODE (y: 43-53)

    _display.setCursor(0, 43);
    _display.print("APP:");

    _display.setCursor(30, 43);
    _display.print(formatSize(appUsed));
    _display.print("/");
    _display.print(formatSize(appTotal));

    int appPct = (appTotal > 0) ? (int)((float)appUsed / appTotal * 100) : 0;
    if(appPct > 100) appPct = 100;

    _display.drawRect(0, 52, 128, 4, SSD1306_WHITE);
    int appBarWidth = map(appPct, 0, 100, 0, 128);
    _display.fillRect(0, 52, appBarWidth, 4, SSD1306_WHITE);
    
    _display.display();
}

void DisplayManager::drawConfirmation(String message, bool yesSelected) {
    _display.clearDisplay();

    _display.fillRect(0, 0, 128, 16, SSD1306_WHITE);
    _display.setTextColor(SSD1306_BLACK);
    _display.setTextSize(1);
    _display.setCursor(25, 4);
    _display.print("ARE YOU SURE?");

    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0,25);

    int msgCenter = (21 - message.length()) * 3;
    if(msgCenter < 0) msgCenter = 0;
    _display.setCursor(msgCenter, 25);
    _display.print(message);

    int yBtn = 48;
    int wBtn = 40;
    int hBtn = 14;

    if(yesSelected) {
        _display.fillRect(10, yBtn, wBtn, hBtn, SSD1306_WHITE);
        _display.setTextColor(SSD1306_BLACK);
    } else {
        _display.drawRect(10, yBtn, wBtn, hBtn, SSD1306_WHITE);
        _display.setTextColor(SSD1306_WHITE);
    }
    _display.setCursor(22, yBtn + 3);
    _display.print("YES");

    if(!yesSelected) {
        _display.fillRect(78, yBtn, wBtn, hBtn, SSD1306_WHITE);
        _display.setTextColor(SSD1306_BLACK);
    } else {
        _display.drawRect(78, yBtn, wBtn, hBtn, SSD1306_WHITE);
        _display.setTextColor(SSD1306_WHITE);
    }
    _display.setCursor(93, yBtn + 3);
    _display.print("NO");
    
    _display.display();
}

void DisplayManager::drawStopwatch(String time, float seconds, bool isPaused) {
    _display.clearDisplay();

    _display.drawLine(0, _headerHeight - 1, SCR_WIDTH, _headerHeight - 1, SSD1306_WHITE);
    _display.setTextSize(1);
    _display.setTextColor(SSD1306_WHITE);
    _display.setCursor(0, 4);
    _display.print("Stopwatch");

    int centerOffset = (17 - time.length()) * 3;
    if(centerOffset < 0) centerOffset = 0;
    _display.setCursor(centerOffset, 25);

    _display.setTextSize(2);
    _display.print(time);

    _display.setTextSize(1);
    _display.setCursor(51, 40);
    _display.print(seconds);

    if(isPaused) {
        _display.setTextSize(1);
        _display.setCursor(55, 50);
        _display.print("P");
    }

    _display.display();
}