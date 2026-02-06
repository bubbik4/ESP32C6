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
    WiFi.setHostname(HOSTNAME);
    WiFi.mode(WIFI_STA);
    WiFiManager wm;

    wm.setConnectTimeout(30);

    wm.setConfigPortalTimeout(180); //s

    wm.setAPCallback([](WiFiManager *newWiFiManager) {
        LOG("[!!] No WIFI connection");
        LOG("Connect to AP: " + String(WIFI_AP_NAME));
    });

    wm.setDebugOutput(true);

    if(!wm.autoConnect(WIFI_AP_NAME, "passwd")) {
        LOG("WiFi Timeout");
    }

    if(isConnected()) {
        LOG("WiFi connected");
        LOGF("IP: %s\n", WiFi.localIP().toString().c_str());
    } else LOG("WiFi not set up");
    WiFi.setSleep(true);
}

void NetManager::setupOTA() {
    if(isConnected()) {
        ArduinoOTA.setHostname(HOSTNAME);

        ArduinoOTA.onStart([]() {
            LOG("OTA Start");
            WiFi.setSleep(false);
        });
        ArduinoOTA.onEnd([]() {
            LOG("\nOTA End");
            WiFi.setSleep(true);
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
    } else LOG("OTA not set up - NO WIFI");
}

void NetManager::setupTime() {
    if(isConnected()) {
        configTime(0, 0, "pool.ntp.org", "time.nist.gov");

        setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1); // Polish timezone
        tzset();

        LOG("Time server UP");
    } else LOG("Time server not connected");
}
