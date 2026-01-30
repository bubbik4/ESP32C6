#include <Arduino.h>

#include "displayManager.h"
#include "networkManager.h"

DisplayManager display;
NetManager net;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");
  
  display.begin();
  display.drawBootScreen();
  net.begin();
}

void loop() {
  net.update();
}


