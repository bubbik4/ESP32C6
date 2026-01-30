#include <Arduino.h>

#include "displayManager.h"
#include "networkManager.h"
#include "Button.h"
#include "config.h"

DisplayManager display;
NetManager net;

Button btnLeft(D7);
Button btnRight(D8);

enum AppState {
  STATE_MENU,
  STATE_CLOCK,
  STATE_POMODORO,
  STATE_STOPWATCH,
  STATE_WIFI,
  STATE_SYSTEM
};

AppState currentState = STATE_MENU;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  btnLeft.begin();
  btnRight.begin();

  display.begin();
  display.drawBootScreen();

  net.begin();

  LOG("System UP");
  display.drawMenu();
}


void loop() {
  net.update();
  btnRight.update();
  btnLeft.update();

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
          display.drawWIP(); //to be implemented
          break;
        case 2: // stopwatch
          LOG("STOPWATCH");
          currentState = STATE_STOPWATCH;
          display.drawWIP(); //to be implemented
          break;
        case 3: // WiFi Info
          LOG("WIFI INFO");
          currentState = STATE_WIFI;
          display.drawWIP(); //to be implemented
          break;
        case 4: // system
          LOG("SYSTEM");
          currentState = STATE_SYSTEM;
          display.drawWIP(); //to be implemented
          break;
      }
    }
  } 
  else if(currentState == STATE_CLOCK) {
    display.updateClock();
  }
  if(currentState != STATE_MENU) {
    if(btnRight.hasJustHeld(800)) {
      LOG("Exiting");
      currentState = STATE_MENU;
      display.drawMenu();
    }

    if(btnRight.hasJustClicked()) {
      LOG("ignoring");
    }
  }
}

