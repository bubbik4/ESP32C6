#include <Arduino.h>

#include "displayManager.h"
#include "networkManager.h"
#include "Button.h"
#include "config.h"

DisplayManager display;
NetManager net;

Button btnLeft(D7);
Button btnRight(D8);

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

  if(btnLeft.isClicked()) {
    LOG("L Click");
    display.moveSelection(1);
  }

  if(btnRight.isClicked()) {
    LOG("R Click");
    //
  }
}

