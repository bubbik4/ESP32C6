#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <WiFiManager.h>
#include <WiFi.h>

#include "config.h"

class NetManager {
public:
    NetManager();

    void begin();
    void update();

    bool isConnected() const { return WiFi.status() == WL_CONNECTED; }

private:
    void setupWiFi();
    void setupTime();
    void setupOTA();
};