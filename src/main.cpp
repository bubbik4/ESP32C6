#include <Arduino.h>

#include "displayManager.h"
#include "networkManager.h"
#include "config.h"

DisplayManager display;
NetManager net;

bool lastBtnState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  pinMode(BTN_LEFT, INPUT_PULLUP);
  
  display.begin();
  display.drawBootScreen();

  net.begin();

  LOG("System UP");
  display.drawMenu();
}


void loop() {
  net.update();
  int reading  = digitalRead(BTN_LEFT);

  if(reading != lastBtnState) {
    lastDebounceTime = millis();
  } 

  if((millis() - lastDebounceTime) > debounceDelay) {

    static int buttonState = HIGH;
    if(reading != buttonState) {
      buttonState = reading;

      if(buttonState == LOW) {
        LOG("Click!");
        display.moveSelection(1);
      }
    }
  }
  lastBtnState = reading;
}