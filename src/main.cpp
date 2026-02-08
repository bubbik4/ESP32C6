#include <Arduino.h>
#include <SPIFFS.h>
#include <functional>

#include "displayManager.h"
#include "networkManager.h"
#include "powerManager.h"
#include "pomodoro.h"
#include "stopwatch.h"
#include "Button.h"
#include "config.h"

DisplayManager display;
NetManager net;
PomodoroTimer pomodoro;
Stopwatch stopwatch;
PowerManager power;

Button btnLeft(D7);
Button btnRight(D8);

bool isAlarmAnimating = false;
int alarmBlinkCount = 0;
unsigned long lastBlinkTime = 0;
bool blinkState = false;

enum AppState {
  STATE_MENU,
  STATE_CLOCK,
  STATE_POMODORO,
  STATE_STOPWATCH,
  STATE_WIFI,
  STATE_SYSTEM,
  STATE_CONFIRM
};

AppState previousState; // where to go when NO chosen
AppState currentState = STATE_MENU; 

String confirmMessage; // what to show on prompt 
std::function<void()> onConfirmAction; // what to do when YES chosen
bool isYesSelected = false; // defaults to NO

void askConfirmation(String msg, std::function<void()> action) {
  previousState = currentState; // remember the last state
  confirmMessage = msg;
  onConfirmAction = action;
  isYesSelected = false; // reset to NO
  currentState = STATE_CONFIRM;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  if(!SPIFFS.begin(true)) {
    LOG("An error occured while mounting SPIFFS");
  }

  power.begin();

  btnLeft.begin();
  btnRight.begin();

  display.begin();
  display.drawBootScreen();

  net.begin();

  stopwatch.begin();

  LOG("System UP");
  display.drawMenu();
}


void loop() {
  net.update();
  power.update();
  btnRight.update();
  btnLeft.update();
  pomodoro.update();


  // TEST
  // LOG(stopwatch.getFormattedTime());
  // ----

  if (pomodoro.isAlarmTriggered()) {
      isAlarmAnimating = true;
      alarmBlinkCount = 0;
  }

  if (isAlarmAnimating) {
      if (millis() - lastBlinkTime > 300) {
          lastBlinkTime = millis();
          blinkState = !blinkState;
          display.invertScreen(blinkState);
          alarmBlinkCount++;
          if (alarmBlinkCount >= 6) { 
              isAlarmAnimating = false;
              display.invertScreen(false);
          }
      }
  }

  if(currentState == STATE_MENU) {

    if(btnLeft.hasJustClicked()) {
      LOG("L Click");
      display.moveSelection(1);
    }

    if(btnRight.hasJustClicked()) {
      LOG("R Click");
      
      int selected = display.getSelectedIndex();

      switch (selected) {
        case 0: // clock
          LOG("CLOCK");
          currentState = STATE_CLOCK;
          display.drawClock();
          break;
        case 1: // pomodoro
          LOG("POMODORO");
          currentState = STATE_POMODORO;
          // pomodoro.reset();
          display.drawPomodoro(pomodoro.getFormattedTime(), pomodoro.getStatusLabel(), pomodoro.getCycleCount());
          break;
        case 2: // stopwatch
          LOG("STOPWATCH");
          currentState = STATE_STOPWATCH;
          // display.drawWIP(); //to be implemented
          break;
        case 3: // WiFi Info
          LOG("WIFI INFO");
          currentState = STATE_WIFI;
          // display.drawWiFiInfo();
          break;
        case 4: // system
          LOG("SYSTEM");
          currentState = STATE_SYSTEM;

          float used = SPIFFS.usedBytes();
          float total = SPIFFS.totalBytes();
          size_t appUsed = ESP.getSketchSize();
          size_t appTotal = ESP.getSketchSize() + ESP.getFreeSketchSpace();
          display.drawSystem(
            power.getVoltage(), 
            power.getPercentage(), 
            used, 
            total, 
            appUsed, 
            appTotal
          );

          break;
      }
    }
  } 

  else if(currentState == STATE_CLOCK) {
    display.updateClock();
  } 

  else if(currentState == STATE_CONFIRM) {
    if(btnLeft.hasJustClicked()) {
      isYesSelected = !isYesSelected;
    }

    if (btnRight.hasJustClicked()) {
      if(isYesSelected) {
        if(onConfirmAction) onConfirmAction();
        LOG("Confirmed: YES");
      } else {
        LOG("Confirmed: NO");
      }
      currentState = previousState; // revert to previous state if NO
    }

    display.drawConfirmation(confirmMessage, isYesSelected);
  }

  else if(currentState == STATE_WIFI) {
    static unsigned long lastWiFiUpdate = 0;
    if(millis() - lastWiFiUpdate > 2000) {//2s
      lastWiFiUpdate = millis();

      display.drawWiFiInfo(
        WiFi.SSID(),
        WiFi.localIP().toString(),
        WiFi.macAddress(),
        WiFi.RSSI(),
        WiFi.status() == WL_CONNECTED
      );

    }
  }

  else if(currentState == STATE_STOPWATCH) {
    stopwatch.updateTimer();
    
    if (btnRight.hasJustClicked()) {
      stopwatch.togglePause();
    }

  }
  
  else if(currentState == STATE_POMODORO) {

    
    if (btnRight.hasJustClicked()) {
      pomodoro.toggleStartPause();
    }

    if(btnLeft.hasJustHeld()) {
      pomodoro.switchState();
    }

    if (btnLeft.hasJustClicked()) {
      askConfirmation("Reset Timer?", []() {
        pomodoro.reset();
      });
    }

    display.drawPomodoro(
      pomodoro.getFormattedTime(),
      pomodoro.getStatusLabel(),
      pomodoro.getCycleCount()
    );
  }

  else if(currentState == STATE_SYSTEM) {
    static unsigned long lastSysUpdate = 0;
    if(millis() - lastSysUpdate > 1000) {
      lastSysUpdate = millis();
      
      float used = SPIFFS.usedBytes();
      float total = SPIFFS.totalBytes();
      size_t appUsed = ESP.getSketchSize();
      size_t appTotal = ESP.getSketchSize() + ESP.getFreeSketchSpace();
      display.drawSystem(
        power.getVoltage(), 
        power.getPercentage(), 
        used, 
        total, 
        appUsed, 
        appTotal
      );
    }
  } 


  if(currentState != STATE_MENU) {
    if(btnRight.hasJustHeld(800)) {
      LOG("Exiting");
      currentState = STATE_MENU;
      display.drawMenu();
    }


  }

}
