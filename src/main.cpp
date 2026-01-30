#include <Arduino.h>

#include "displayManager.h"

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing...");

  DisplayManager display;

  display.drawBootScreen();
}

void loop() {
  
}
