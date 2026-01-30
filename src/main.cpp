#include <Arduino.h>

#include "displayManager.h"
#include "networkManager.h"
#include "config.h"

DisplayManager display;
NetManager net;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");
  
  display.begin();
  display.drawBootScreen();

  net.begin();

  LOG("System UP");
}


void loop() {
  net.update();
  // temp main screen
  // display.drawMenu();

  delay(1000);
  display.moveSelection(1);

}