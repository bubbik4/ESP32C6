#include "networkManager.h"

NetManager::NetManager() {}

void NetManager::begin() {
    setupWiFi();
    setupOTA();
    setupTime();
}

void NetManager::update() {
    ArduinoOTA.handle();
}

void NetManager::setupWiFi() {
    WiFi.mode(WIFI_STA);
    WiFiManager wm;

    wm.setConnectTimeout(30);

    wm.setConfigPortalTimeout(180); //s

    wm.setAPCallback([](WiFiManager *newWiFiManager) {
        LOG("[!!] No WIFI connection");
        LOG("Connect to AP: " + String(WIFI_AP_NAME));
    });

    wm.setDebugOutput(true);

    if(!wm.autoConnect(WIFI_AP_NAME)) {
        LOG("WiFi Timeout");
        // restart
    }

    LOG("WiFi connected");
    LOGF("IP: %s\n", WiFi.localIP().toString().c_str());
}

void NetManager::setupOTA() {
    ArduinoOTA.setHostname(HOSTNAME);

    ArduinoOTA.onStart([]() {
        LOG("OTA Start");
    });
    ArduinoOTA.onEnd([]() {
        LOG("\nOTA End");
    });
    ArduinoOTA.onError([](ota_error_t error) {
        LOGF("Error[%u]: ", error);
        if(error == OTA_AUTH_ERROR) LOG("Auth Failed");
        else if(error == OTA_BEGIN_ERROR) LOG("Begin Failed");
        else if(error == OTA_CONNECT_ERROR) LOG("Connect Failed");
        else if(error == OTA_RECEIVE_ERROR) LOG("Recieve Failed");
        else if(error == OTA_END_ERROR) LOG("End Failed");
    });
    ArduinoOTA.begin();
}

void NetManager::setupTime() {
    configTime(3600, 3600, "pool.ntp.org");
    LOG("Time server UP");
}